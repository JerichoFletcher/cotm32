#pragma once

#include "kernel/task.h"
#include "trap/enums.h"
#include "int.h"

/// @brief The maximum number of tasks the kernel can keep track of.
#define MAX_TASKS 8U

extern Task tasks[];
extern size_t current_tid;

/// @brief Gets the current active task.
/// @return A pointer to the current task.
static inline Task* current_task(void) {
    return &tasks[current_tid];
}

/// @brief Starts the schedule at an entrypoint. Will never return.
/// @param entrypoint The first task to be executed.
void start_schedule(Task* entrypoint) __attribute__((noreturn));

/// @brief Allocate a task slot and marks it as "ready".
/// @param priority The priority of the task.
/// @return A pointer to the newly allocated task, or `NULL` if allocation fails.
Task* alloc_new_task(size_t priority);

/// @brief Schedules the next waiting task.
/// @brief Will select tasks based on priority with round-robin selection for equal priority tasks.
void schedule(void);

/// @brief Marks the given task as "blocked with IRQ".
/// @param task The task to block.
/// @param interr Which interrupt should wake the task up.
void block_task_irq(Task* task, Interrupt interr);

/// @brief Wakes blocked tasks that requests the given interrupt.
/// @param interr The interrupt code.
/// @return The number of awaken tasks.
size_t wake_irq_tasks(Interrupt interr);
