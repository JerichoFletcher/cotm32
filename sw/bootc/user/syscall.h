#pragma once

#include "int.h"
#include "sys/enums.h"

/// @brief Exits the current task.
void exit(void)
__attribute__((noreturn));

/// @brief Yields execution of the current task.
void yield(void);

/// @brief Frees a heap-allocated memory.
/// @param ptr The pointer to the memory.
void free(void* ptr);

/// @brief Allocates a memory space in the heap.
/// @param size The size of the requested memory.
/// @return A pointer to the memory, or `NULL` if allocation fails.
void* malloc(size_t size)
__attribute__((malloc, malloc(free, 1)));

/// @brief Writes a character to the terminal.
/// @param c The character.
void putc(char c);

/// @brief Reads a character from the terminal.
/// @return A character.
char getc(void);

/// @brief Writes a null-terminated string to the terminal with a newline ending (`\n`).
/// @param s The string buffer.
void puts(const char* s);
