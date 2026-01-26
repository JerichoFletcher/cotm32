#pragma once

#include "context.h"
#include "kernel/stack.h"
#include "priv/enums.h"
#include "trap/trap.h"
#include "bool.h"

/// @brief A task identifier.
typedef int32_t task_id_t;

/// @brief Constructs a task ID from its slot index and generation number.
#define TASK_ID(slot, gen)  (((gen) << 8) | (slot))
/// @brief Extracts the generation number from a task ID.
#define TASK_GEN(id)        ((id) >> 8)
/// @brief Extracts the slot index from a task ID.
#define TASK_SLOT(id)       ((id) & 0xff)

/// @brief The minimum number of timer ticks in a quantum.
#define SCHED_QUANTUM_TICKS         10
/// @brief The amount of bonus ticks per priority level.
#define SCHED_PRIORITY_BONUS_TICKS  1

/// @brief The maximum number of tasks the kernel can keep track of.
#define MAX_TASKS 8U

/// @brief The state of a task. Determines how the scheduler manages a task.
typedef enum TaskState {
    /// @brief Denotes that the current task slot is not allocated.
    TaskState_NOT_CREATED = 0,

    /// @brief Denotes that the current task is ready to be executed.
    TaskState_READY,

    /// @brief Denotes that the current task is active and running.
    /// @brief There should only ever be at most one task with this state at all times.
    TaskState_RUNNING,

    /// @brief Denotes that the current task is waiting for an interrupt.
    TaskState_BLOCKED_IRQ,

    /// @brief Denotes that the current task is terminated.
    /// @brief Terminated tasks will be cleared on the next scheduler tick.
    TaskState_TERMINATED,
} TaskState;

/// @brief Descriptor for a task unit.
typedef struct Task {
    /// @brief The processor context for this task.
    Context ctx;

    /// @brief The ID of this task.
    task_id_t id;

    /// @brief The descriptor for the stack allocated to this task.
    StackDescriptor* stack;

    /// @brief The priority of the task.
    /// @brief Higher-priority tasks are executed before lower-priority ones.
    size_t priority;

    /// @brief The remaining amount of timer ticks in the allotted quantum for this task.
    size_t time_slice;

    /// @brief The state of this task.
    TaskState state;
} Task;

/// @brief Gets the ID of the current task.
/// @return The ID of the current task.
task_id_t current_task(void);

/// @brief Sets a task as the current active one.
/// @param tid The ID the task.
void set_current_task(task_id_t tid);

/// @brief Spawns a new task.
/// @param entry The entrypoint of the task.
/// @param priv Which privilege mode to run the task in.
/// @param priority The priority of the task.
/// @return The ID of the created task, or `-1` if failed.
task_id_t spawn_task(void (*entry)(void), PrivMode priv, size_t priority);

/// @brief Enters a task.
/// @brief This will discard the caller's context. As such, this is a one-way operation.
/// @param tid The ID of the task.
void enter_task(task_id_t tid)
__attribute__((noreturn));

/// @brief Destroys a task.
/// @brief This will immediately deallocate the task slot and free any resources it holds.
/// @param tid The ID of the task.
void destroy_task(task_id_t tid);

/// @brief Checks if a task exists.
/// @param tid The ID of the task.
/// @return `TRUE` if the task exists.
bool_t task_exists(task_id_t tid);

/// @brief Gets the state of a task.
/// @param tid The ID of the task.
/// @param out The state of the task.
/// @return `TRUE` if the task exists.
bool_t get_task_state(task_id_t tid, TaskState* out)
__attribute__((access(write_only, 2)));

/// @brief Gets the priority of a task.
/// @param tid The ID of the task.
/// @param out The priority of the task.
/// @return `TRUE` if the task exists.
bool_t get_task_priority(task_id_t tid, size_t* out)
__attribute__((access(write_only, 2)));

/// @brief Gets the current value of the time slice of a task.
/// @param tid The ID of the task.
/// @param out The remaining time slice of the task.
/// @return `TRUE` if the task exists.
bool_t get_task_time_slice(task_id_t tid, size_t* out)
__attribute__((access(write_only, 2)));

/// @brief Sets the task state to `READY`.
/// @param tid The ID of the task.
void set_task_ready(task_id_t tid);

/// @brief Sets the task state to `RUNNING`.
/// @param tid The ID of the task.
void set_task_running(task_id_t tid);

/// @brief Sets the task state to `BLOCKED_IRQ`.
/// @param tid The ID of the task.
void set_task_blocked_irq(task_id_t tid);

/// @brief Sets the time slice of the task.
/// @param tid The ID of the task.
/// @param time_slice The new time slice of the task.
void set_task_time_slice(task_id_t tid, size_t time_slice);

/// @brief Switches the context to the next scheduled task.
/// @param ctx A pointer to the current task's active context.
void switch_to_next_task(Context* ctx)
__attribute__((access(read_write, 1)));

/// @brief Marks the given task as "terminated".
/// @brief The task will be cleared on the next scheduler tick.
/// @param tid The ID of the task.
void terminate_task(task_id_t tid);
