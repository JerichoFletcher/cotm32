#pragma once
#include "kernel/heap.h"

/// @brief Gets the global heap descriptor. Should only be used to allocate task-level heaps.
/// @return A pointer to the global heap descriptor.
HeapDescriptor* get_global_heap(void);
