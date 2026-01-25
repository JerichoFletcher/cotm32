#include "kernel/scheduler.h"
#include "kernel/task.h"
#include "kernel/stack.h"
#include "trap/trap.h"
#include "csr.h"
#include "context.h"
#include "bool.h"

#include "sys/ksys.h"
#include "panic.h"

typedef struct TaskReadyEntry {
    task_id_t task_id;
    struct TaskReadyEntry* prev;
    struct TaskReadyEntry* next;
} TaskReadyEntry;

TaskReadyEntry qready[MAX_TASKS] = {0};
TaskReadyEntry* qready_head = NULL;
size_t n_ready = 0;

typedef struct TaskBlockedEntry {
    task_id_t task_id;
    size_t wait_irq_mask;
    bool_t blocked;
} TaskBlockedEntry;

TaskBlockedEntry qblocked[MAX_TASKS] = {0};
size_t n_blocked = 0;

bool_t push_ready_task(TaskReadyEntry* entr) {
    if (entr == NULL) return FALSE;
    if (n_ready == MAX_TASKS) return FALSE;

    size_t entr_prio; get_task_priority(entr->task_id, &entr_prio);
    size_t head_prio; get_task_priority(qready_head->task_id, &head_prio);
    if (n_ready == 0 || entr_prio > head_prio) {
        entr->prev = NULL;
        entr->next = qready_head;
        qready_head = entr;
    } else {
        TaskReadyEntry* prev = qready_head;
        TaskReadyEntry* next = prev->next;
        
        size_t curr_prio;
        size_t next_prio;
        get_task_priority(entr->task_id, &curr_prio);
        if (next != NULL) {
            get_task_priority(next->task_id, &next_prio);
        }
        
        // Search prev and next such that PRIO(prev) >= PRIO(curr) > PRIO(next)
        while (next != NULL && next_prio >= curr_prio) {
            prev = next;
            next = prev->next;

            get_task_priority(entr->task_id, &curr_prio);
            if (next != NULL) {
                get_task_priority(next->task_id, &next_prio);
            }
        }

        // Link with the prev and next entry
        entr->prev = prev;
        entr->next = next;

        prev->next = entr;
        if (next != NULL) next->prev = entr;
    }
    n_ready++;
    return TRUE;
}

bool_t remove_ready_task(TaskReadyEntry* entr) {
    if (entr == NULL) return FALSE;

    if (entr->prev != NULL) {
        entr->prev->next = entr->next;
    } else if (entr == qready_head) {
        qready_head = qready_head->next;
    }

    if (entr->next != NULL) {
        entr->next->prev = entr->prev;
    }

    entr->prev = NULL;
    entr->next = NULL;
    n_ready--;
    return TRUE;
}

bool_t add_task_to_schedule(task_id_t tid) {
    TaskReadyEntry* curr = &qready[TASK_SLOT(tid)];
    curr->task_id = tid;
    return push_ready_task(curr);
}

void start_schedule(task_id_t tid) {
    TaskState state;
    if (!get_task_state(tid, &state) || state != TaskState_READY) {
        k_puts("Panic: invalid entrypoint task\n", 31);
        panic();
    }
    set_current_task(tid);
    set_task_running(tid);

    if (!remove_ready_task(&qready[TASK_SLOT(tid)])) {
        k_puts("Panic: failed to remove task from queue\n", 40);
        panic();
    }
    enter_task(tid);
}

void schedule(void) {
    task_id_t tid_curr = current_task();
    TaskState state_curr;
    if (!get_task_state(tid_curr, &state_curr)) {
        k_puts("Panic: invalid current task detected\n", 37);
        panic();
    }

    if (state_curr == TaskState_RUNNING) {
        set_task_ready(tid_curr);
        get_task_state(tid_curr, &state_curr);
    }
    
    if (state_curr == TaskState_TERMINATED) {
        destroy_task(tid_curr);
    } else if (state_curr == TaskState_READY) {
        if (!push_ready_task(&qready[TASK_SLOT(tid_curr)])) {
            k_puts("Panic: Failed to push task into queue\n", 38);
            panic();
        }
    }

    if (n_ready > 0) {
        TaskReadyEntry* next = qready_head;
        size_t count = 0;

        while (next != NULL) {
            if (next->task_id == current_task()) {
                // Prefer another task if available to prevent starving
                if (next->next != NULL) {
                    next = next->next;
                    count++;
                }
            }

            task_id_t tid_next = next->task_id;
            TaskState state_next;

            if (get_task_state(tid_next, &state_next) && state_next == TaskState_READY) {
                if (!remove_ready_task(next)) {
                    k_puts("Panic: failed to remove task from queue\n", 40);
                    panic();
                }

                set_current_task(tid_next);
                set_task_running(tid_next);
                size_t t_time;
                if (get_task_time_slice(tid_next, &t_time) && t_time == 0) {
                    size_t t_prio;
                    get_task_priority(tid_next, &t_prio);
                    set_task_time_slice(tid_next, SCHED_QUANTUM_TICKS + t_prio * SCHED_PRIORITY_BONUS_TICKS);
                }

                // Include all requested interrupts in mie
                size_t mie = bits_interr(Interrupt_M_TIMER);
                for (size_t i = 0; i < MAX_TASKS; i++) {
                    TaskBlockedEntry* b = &qblocked[i];
                    if (b->blocked) {
                        mie |= b->wait_irq_mask;
                    }
                }
                assign_mie(mie);
                return;
            }
            next = next->next;
            count++;

            if (count > MAX_TASKS) {
                k_puts("Panic: any task in queue inspected more than once\n", 51);
                panic();
            }
        }
        k_puts("Panic: ran out of tasks\n", 24);
        panic();
    } else {
        k_puts("Panic: invalid task queue state\n", 32);
        panic();
    }
}

void block_task_irq(task_id_t tid, Interrupt interr) {
    if (!task_exists(tid)) return;

    TaskBlockedEntry* entr = &qblocked[TASK_SLOT(tid)];
    entr->blocked = TRUE;
    entr->task_id = tid;

    set_task_blocked_irq(tid);
    entr->wait_irq_mask |= bits_interr(interr);
}

size_t wake_irq_tasks(Interrupt interr) {
    size_t interr_bit = bits_interr(interr);
    size_t num_awaken = 0;

    for (size_t i = 0; i < MAX_TASKS; i++) {
        TaskBlockedEntry* entr = &qblocked[i];
        if (!entr->blocked) continue;

        TaskState state;
        get_task_state(entr->task_id, &state);
        if (state != TaskState_BLOCKED_IRQ) {
            k_puts("Panic: non-blocked task in block queue\n", 39);
            panic();
        }

        if (entr->wait_irq_mask & interr_bit) {
            entr->wait_irq_mask = 0;
            entr->blocked = FALSE;

            set_task_ready(entr->task_id);
            if (!push_ready_task(&qready[TASK_SLOT(entr->task_id)])) {
                k_puts("Panic: failed to move blocked task into the ready queue\n", 56);
                panic();
            }

            num_awaken++;
        }
    }
    return num_awaken;
}
