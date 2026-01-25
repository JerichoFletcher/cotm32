#pragma once

#include "kernel/task.h"
#include "trap/enums.h"
#include "int.h"

/// @brief Adds a new task to the schedule.
/// @param tid The task ID.
/// @return 
bool_t add_task_to_schedule(task_id_t tid);

/// @brief Starts the schedule at an entrypoint. Will never return.
/// @param tid The ID of the first task to be executed.
void start_schedule(task_id_t tid) __attribute__((noreturn));

/// @brief Schedules the next waiting task.
/// @brief Will select tasks based on priority with round-robin selection for equal priority tasks.
void schedule(void);

/// @brief Marks the given task as "blocked with IRQ".
/// @param task The ID of the task to block.
/// @param interr Which interrupt should wake the task up.
void block_task_irq(task_id_t tid, Interrupt interr);

/// @brief Wakes blocked tasks that requests the given interrupt.
/// @param interr The interrupt code.
/// @return The number of awaken tasks.
size_t wake_irq_tasks(Interrupt interr);
