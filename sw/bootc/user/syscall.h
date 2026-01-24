#pragma once

#include "sys/enums.h"
#include "int.h"

/// @brief Exits the current task.
void exit(void) __attribute__((noreturn));

/// @brief Yields execution of the current task. 
void yield(void);

/// @brief Puts a character to the terminal.
/// @param c The character.
void putc(char c);

/// @brief Reads a character from the terminal.
/// @return A character.
char getc(void);

/// @brief Writes a string to the terminal.
/// @param s The string buffer.
/// @param len The length of the string.
void puts(const char* s, size_t len);
