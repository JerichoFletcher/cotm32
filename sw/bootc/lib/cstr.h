#pragma once

#include "int.h"

/// @brief Copies `count` bytes from `src` to `dest`.
/// @param dest The destination memory location.
/// @param src The source memory location.
/// @param count The number of bytes to copy.
/// @return The pointer `dest`.
void* memcpy(void* restrict dest, const void* restrict src, size_t count)
__attribute__((access(write_only, 1), access(read_only, 2), nonnull(1, 2)));

/// @brief Copies `ch` into each of the first `count` bytes starting from `dest`.
/// @param dest The destination memory location.
/// @param ch Fill byte.
/// @param count The number of bytes to fill.
/// @return The pointer `dest`.
void* memset(void* dest, uint8_t ch, size_t count)
__attribute__((access(write_only, 1), nonnull(1)));
