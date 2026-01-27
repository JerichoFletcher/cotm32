#include "kernel/heap.h"

#include "sys/ksys.h"
#include "panic.h"

#define HEAP_ALIGN      (4 * sizeof(size_t))
#define HEAP_MIN_ALLOC  (sizeof(size_t))

typedef struct HeapBlock {
    size_t size;
    bool_t allocated;
    struct HeapBlock* prev;
    struct HeapBlock* next;
} HeapBlock;

bool_t split_block(HeapBlock* block, size_t mem_size) {
    if (block->size < sizeof(HeapBlock) + mem_size + HEAP_MIN_ALLOC) return FALSE;

    size_t remaining = block->size - mem_size;
    if (remaining > sizeof(HeapBlock) + HEAP_MIN_ALLOC) {
        uint8_t* payload = (uint8_t*)(block + 1);
        HeapBlock* insert = (HeapBlock*)(payload + mem_size);
        insert->size = remaining - sizeof(HeapBlock);
        insert->allocated = FALSE;

        insert->prev = block;
        insert->next = block->next;
        if (insert->next != NULL) {
            insert->next->prev = insert;
        }

        block->size = mem_size;
        block->next = insert;
    }
    return TRUE;
}

void merge_empty_blocks_adjacent(HeapBlock* block) {
    if (block == NULL || block->allocated) return;
    HeapBlock* curr = block;

    if (curr->prev != NULL && !curr->prev->allocated) {
        curr->prev->size += sizeof(HeapBlock) + curr->size;
        curr->prev->next = curr->next;
        curr = curr->prev;
        if (curr->next != NULL) {
            curr->next->prev = curr;
        }
    }

    if (curr->next != NULL && !curr->next->allocated) {
        curr->size += sizeof(HeapBlock) + curr->next->size;
        curr->next = curr->next->next;
        if (curr->next != NULL) {
            curr->next->prev = curr;
        }
    }
}

HeapDescriptor init_heap(void* base_addr, size_t size) {
    HeapDescriptor heap;
    heap.head = (void*)ALIGN_UP((size_t)base_addr, HEAP_ALIGN);
    heap.max = (void*)ALIGN_UP((size_t)base_addr + size, HEAP_ALIGN);
    heap.size = (size_t)heap.max - (size_t)heap.head;
    heap.total_allocated = 0;

    HeapBlock* head = heap.head;
    head->size = heap.size - sizeof(HeapBlock);
    head->allocated = FALSE;
    head->prev = NULL;
    head->next = NULL;

    return heap;
}

bool_t is_valid_heap_ptr(HeapDescriptor* heap, void* ptr) {
    return ptr != NULL &&
        (size_t)ptr % HEAP_ALIGN == 0 &&
        (size_t)(heap->head + 1) <= (size_t)ptr &&
        (size_t)ptr <= (size_t)heap->max - sizeof(HeapBlock);
}

void* alloc_heap(HeapDescriptor* heap, size_t size) {
    if (heap->total_allocated + sizeof(HeapBlock) + size > heap->size) return NULL;
    
    HeapBlock* curr = heap->head;
    while (curr != NULL) {
        if (!curr->allocated && split_block(curr, ALIGN_UP(size, HEAP_ALIGN))) {
            curr->allocated = TRUE;
            heap->total_allocated += sizeof(HeapBlock) + ALIGN_UP(size, HEAP_ALIGN);
            return (void*)(curr + 1);
        }
        curr = curr->next;
    }
    return NULL;
}

void free_heap(HeapDescriptor* heap, void* ptr) {
    if (!is_valid_heap_ptr(heap, ptr)) {
        k_puts("Panic: invalid heap pointer");
        panic();
    }

    HeapBlock* block = ((HeapBlock*)ptr) - 1;
    if (!block->allocated) {
        k_puts("Panic: double free");
        panic();
    }
    block->allocated = FALSE;
    heap->total_allocated -= sizeof(HeapBlock) + ALIGN_UP(block->size, HEAP_ALIGN);
    merge_empty_blocks_adjacent(block);
}
