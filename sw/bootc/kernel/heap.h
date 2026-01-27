#pragma once

#include "bool.h"
#include "int.h"

/// @brief Descriptor for a heap space.
typedef struct HeapDescriptor {
    /// @brief The address to the first block in the heap.
    void* head;

    /// @brief The highest address in the heap space.
    void* max;

    /// @brief The size of the heap space, in bytes.
    size_t size;

    /// @brief The total amount of allocated heap memory, in bytes.
    size_t total_allocated;
} HeapDescriptor;

/// @brief Initializes the heap space.
/// @param base_addr The starting address of the heap space.
/// @param size The total size of the heap space.
/// @return A heap descriptor.
HeapDescriptor init_heap(void* base_addr, size_t size) __attribute__((nonnull(1)));

/// @brief Checks if a pointer is a valid heap pointer.
/// @param heap The heap descriptor.
/// @param ptr A pointer.
/// @return `TRUE` if `ptr` points to a valid heap block.
bool_t is_valid_heap_ptr(HeapDescriptor* heap, void* ptr) __attribute__((access(read_only, 1)));

/// @brief Deallocates the heap block.
/// @param heap The heap descriptor.
/// @param ptr A non-null pointer to the heap block.
void free_heap(HeapDescriptor* heap, void* ptr) __attribute__((access(read_write, 1), nonnull(2)));

/// @brief Allocate a block of memory in the heap space.
/// @param heap The heap descriptor.
/// @param size The size of the requested block.
/// @return A pointer to the lowest address of the block, or `NULL` if allocation fails.
void* alloc_heap(HeapDescriptor* heap, size_t size)
    __attribute__((access(read_write, 1), malloc, malloc(free_heap, 2)));
