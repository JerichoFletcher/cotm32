#pragma once

#include "context.h"
#include "priv/enums.h"
#include "trap/trap.h"
#include "bool.h"

/// @brief The minimum number of timer ticks in a quantum.
#define SCHED_QUANTUM_TICKS         10
/// @brief The amount of bonus ticks per priority level.
#define SCHED_PRIORITY_BONUS_TICKS  1

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
    size_t id;

    /// @brief The base address of the stack space.
    size_t stack_base;

    /// @brief The size of the stack space.
    size_t stack_size;

    /// @brief The priority of the task.
    /// @brief Higher-priority tasks are executed before lower-priority ones.
    size_t priority;

    /// @brief The remaining amount of timer ticks in the allotted quantum for this task.
    size_t time_slice;

    /// @brief The state of this task.
    TaskState state;
} Task;

/// @brief Allocates and initializes a new task.
/// @param entry The entrypoint of the task.
/// @param priority The priority of the task.
/// @param priv Which privilege mode to run the task in.
/// @return A pointer to the task descriptor, or `NULL` if allocation fails.
Task* create_task(void (*entry)(void), size_t priority, PrivMode priv);

/// @brief Sets `mstatus.MPIE` for the task.
/// @brief Determines whether interrupt is globally enabled the next time the task is entered.
/// @param task The task.
/// @param mpie The new value of `mstatus.MPIE`.
void task_set_mpie(Task* task, bool_t mpie);

/// @brief Marks the given task as "terminated".
/// @brief The task will be cleared on the next scheduler tick.
/// @param task The task.
void terminate_task(Task* task);
