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

HeapBlock* mem_head;
void* mem_max;

size_t max_size = 0;
size_t total_allocated = 0;

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

void init_heap(void* base_addr, size_t size) {
    mem_head = (HeapBlock*)ALIGN_UP((size_t)base_addr, HEAP_ALIGN);
    mem_max = (void*)ALIGN_UP((size_t)base_addr + size, HEAP_ALIGN);
    max_size = (size_t)mem_max - (size_t)mem_head;

    mem_head->size = max_size - sizeof(HeapBlock);
    mem_head->allocated = FALSE;
    mem_head->prev = NULL;
    mem_head->next = NULL;
}

bool_t is_valid_heap_ptr(void* ptr) {
    return ptr != NULL &&
        (size_t)ptr % HEAP_ALIGN == 0 &&
        (size_t)(mem_head + 1) <= (size_t)ptr &&
        (size_t)ptr <= (size_t)mem_max - sizeof(HeapBlock);
}

void* allocate_heap(size_t size) {
    if (total_allocated + sizeof(HeapBlock) + size > max_size) return NULL;
    
    HeapBlock* curr = mem_head;
    while (curr != NULL) {
        if (!curr->allocated && split_block(curr, ALIGN_UP(size, HEAP_ALIGN))) {
            curr->allocated = TRUE;
            total_allocated += sizeof(HeapBlock) + ALIGN_UP(size, HEAP_ALIGN);
            return (void*)(curr + 1);
        }
        curr = curr->next;
    }
    return NULL;
}

void free_heap(void* ptr) {
    if (!is_valid_heap_ptr(ptr)) {
        k_puts("Panic: invalid heap pointer\n", 28);
        panic();
    }

    HeapBlock* block = ((HeapBlock*)ptr) - 1;
    if (!block->allocated) {
        k_puts("Panic: double free\n", 19);
        panic();
    }
    block->allocated = FALSE;
    total_allocated -= sizeof(HeapBlock) + ALIGN_UP(block->size, HEAP_ALIGN);
    merge_empty_blocks_adjacent(block);
}
