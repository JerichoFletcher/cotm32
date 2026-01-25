#pragma once

#include "context.h"
#include "int.h"

/// @brief The status of a system call.
/// @brief Controls how the dispatcher will handle transition from the system call.
typedef enum SyscallStatus {
    /// @brief The system call is done, and the task is free to move to the next instruction.
    SyscallStatus_DONE,
    /// @brief The system call should be retried. Prevents PC advance and task yielding.
    SyscallStatus_RETRY,
    /// @brief The system call is blocked and the task should yield execution.
    SyscallStatus_BLOCKED,
} SyscallStatus;

/// @brief Terminates the current task.
/// @param ctx The context of the syscall.
/// @return Always returns `DONE`.
SyscallStatus k_exit(Context* ctx);

/// @brief Yields execution of the current task.
/// @param ctx The context of the syscall.
/// @return Always returns `DONE`.
SyscallStatus k_yield(Context* ctx);

/// @brief Frees a heap-allocated memory.
/// @param ptr The pointer to the memory.
/// @return Always returns `DONE`.
SyscallStatus k_free(void* ptr);

/// @brief Allocates a memory space in the heap.
/// @param size The size of the requested space, in bytes.
/// @param out_ptr The pointer to the newly allocated memory.
/// @return Always returns `DONE`.
SyscallStatus k_malloc(size_t size, void** out_ptr);

/// @brief Writes a character to the terminal.
/// @param c The character to write.
/// @return `DONE` if the write is successful, otherwise `RETRY`.
SyscallStatus k_putc(char c);

/// @brief Reads a character from the terminal.
/// @brief If no data is available, blocks the current task.
/// @param out_c The read character.
/// @return `DONE` if the read is successful, otherwise `BLOCK`.
SyscallStatus k_getc(char* out_c);

/// @brief Writes a string to the terminal.
/// @param s A pointer to the first character in the buffer.
/// @param len The length of the string.
/// @return Always returns `DONE`.
SyscallStatus k_puts(const char* s, size_t len);
