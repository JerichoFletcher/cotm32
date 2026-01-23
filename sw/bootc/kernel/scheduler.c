#include "kernel/scheduler.h"
#include "kernel/task.h"
#include "kernel/stack.h"
#include "trap/trap.h"
#include "csr.h"
#include "context.h"
#include "bool.h"

#include "sys/ksys.h"
#include "panic.h"

Task tasks[MAX_TASKS] = {0};
size_t current_tid = 0;
size_t n_task = 0;

typedef struct TaskReadyEntry {
    Task* task;
    struct TaskReadyEntry* prev;
    struct TaskReadyEntry* next;
} TaskReadyEntry;

TaskReadyEntry qready[MAX_TASKS] = {0};
TaskReadyEntry* qready_head = NULL;
size_t n_ready = 0;

typedef struct TaskBlockedEntry {
    Task* task;
    size_t wait_irq_mask;
    bool_t blocked;
} TaskBlockedEntry;

TaskBlockedEntry qblocked[MAX_TASKS] = {0};
size_t n_blocked = 0;

bool_t push(TaskReadyEntry* entr) {
    if (entr == NULL) return FALSE;
    if (n_ready == MAX_TASKS) return FALSE;

    if (n_ready == 0 || entr->task->priority > qready_head->task->priority) {
        entr->prev = NULL;
        entr->next = qready_head;
        qready_head = entr;
    } else {
        TaskReadyEntry* prev = qready_head;
        TaskReadyEntry* next = prev->next;

        // Search prev and next such that PRIO(prev) >= PRIO(curr) > PRIO(next)
        while (next != NULL && next->task->priority >= entr->task->priority) {
            prev = next;
            next = prev->next;
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

bool_t remove(TaskReadyEntry* entr) {
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

void start_schedule(Task* entrypoint) {
    if (entrypoint == NULL ||
        entrypoint->state != TaskState_READY ||
        entrypoint != &tasks[entrypoint->id]
    ) {
        k_puts("Invalid entrypoint task\n", 24);
        panic();
    }
    current_tid = entrypoint->id;
    entrypoint->state = TaskState_RUNNING;
    remove(&qready[entrypoint->id]);
    enter_context(&entrypoint->ctx);
}

Task* alloc_new_task(size_t priority) {
    if (n_task == MAX_TASKS) return NULL;
    
    // Find an unallocated task entry
    Task* task = NULL;
    size_t tid;
    for (tid = 0; tid < MAX_TASKS; tid++) {
        if (tasks[tid].state == TaskState_NOT_CREATED) {
            task = &tasks[tid];
            break;
        }
    }
    if (!task) return NULL;
    
    task->id = tid;
    task->priority = priority;

    // Push task into the ready queue
    TaskReadyEntry* curr = &qready[tid];
    curr->task = task;
    if (!push(curr)) return NULL;

    task->state = TaskState_READY;
    n_task++;
    return task;
}

void clear_task(Task* task) {
    if (task->state != TaskState_NOT_CREATED) {
        free_stack(task->id);
        task->state = TaskState_NOT_CREATED;
        n_task--;
    }
}

void schedule(void) {
    if (current_task()->state == TaskState_RUNNING) {
        current_task()->state = TaskState_READY;
    }
    
    if (current_task()->state == TaskState_TERMINATED) {
        clear_task(current_task());
    } else if (current_task()->state == TaskState_READY) {
        if (!push(&qready[current_task()->id])) {
            k_puts("Panic: Failed to push task into queue\n", 38);
            panic();
        }
    }

    if (n_ready > 0) {
        TaskReadyEntry* curr = qready_head;
        size_t count = 0;

        while (curr != NULL) {
            if (curr->task == current_task()) {
                // Prefer another task if available to prevent starving
                if (curr->next != NULL) {
                    curr = curr->next;
                    count++;
                }
            }

            Task* task = curr->task;
            if (task->state == TaskState_READY) {
                if (!remove(curr)) {
                    k_puts("Panic: failed to remove task from queue\n", 40);
                    panic();
                }

                current_tid = task->id;
                task->state = TaskState_RUNNING;
                if (task->time_slice == 0) {
                    task->time_slice = SCHED_QUANTUM_TICKS + task->priority * SCHED_PRIORITY_BONUS_TICKS;
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
            curr = curr->next;
            count++;

            if (count > MAX_TASKS) {
                k_puts("Panic: any task in queue inspected more than once\n", 51);
                panic();
            }
        }
        k_puts("Panic: ran out of tasks\n", 24);
        panic();
    } else {
        k_puts("Panic: invalid task queue state\n", 26);
        panic();
    }
}

void block_task_irq(Task* task, Interrupt interr) {
    TaskBlockedEntry* entr = &qblocked[task->id];
    entr->blocked = TRUE;
    entr->task = task;

    task->state = TaskState_BLOCKED_IRQ;
    entr->wait_irq_mask |= bits_interr(interr);
}

size_t wake_irq_tasks(Interrupt interr) {
    size_t interr_bit = bits_interr(interr);
    size_t num_awaken = 0;

    for (size_t i = 0; i < MAX_TASKS; i++) {
        TaskBlockedEntry* entr = &qblocked[i];
        if (!entr->blocked) continue;

        if (entr->task->state != TaskState_BLOCKED_IRQ) {
            k_puts("Panic: non-blocked task in block queue: ", 40);
            panic();
        }

        if (entr->wait_irq_mask & interr_bit) {
            entr->wait_irq_mask = 0;
            entr->blocked = FALSE;

            entr->task->state = TaskState_READY;
            push(&qready[entr->task->id]);

            num_awaken++;
        }
    }
    return num_awaken;
}
