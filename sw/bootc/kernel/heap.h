#pragma once

#include "bool.h"
#include "int.h"

/// @brief Initializes the heap space.
/// @param base_addr The starting address of the heap space.
/// @param size The total size of the heap space.
void init_heap(void* base_addr, size_t size);

/// @brief Checks if a pointer is a valid heap pointer.
/// @param ptr A pointer.
/// @return `TRUE` if `ptr` points to a valid heap block.
bool_t is_valid_heap_ptr(void* ptr);

/// @brief Deallocates the heap block.
/// @param ptr A non-null pointer to the heap block.
void free_heap(void* ptr)
__attribute__((nonnull(1)));

/// @brief Allocate a block of memory in the heap space.
/// @param size The size of the requested block.
/// @return A pointer to the lowest address of the block, or `NULL` if allocation fails.
void* allocate_heap(size_t size)
__attribute__((malloc, malloc(free_heap, 1)));
