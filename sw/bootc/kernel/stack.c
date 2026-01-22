#include "kernel/stack.h"

#define MAX_STACK_SLOT      8
#define STACK_BASE_ADDR     0x80001000
#define STACK_SIZE          0x1000

typedef struct StackSlot {
    Stack stack;
    size_t owner_tid;
    bool_t allocated;
} StackSlot;

StackSlot slots[MAX_STACK_SLOT] = {0};
size_t n_slot;

Stack* alloc_stack(size_t owner_tid) {
    if (n_slot == MAX_STACK_SLOT) return FALSE;

    for (size_t i = 0; i < MAX_STACK_SLOT; i++) {
        StackSlot* slot = &slots[i];
        if (!slot->allocated) {
            slot->allocated = TRUE;
            slot->owner_tid = owner_tid;

            Stack* stack = &slot->stack;
            stack->base = STACK_BASE_ADDR + i * STACK_SIZE;
            stack->size = STACK_SIZE;
            n_slot++;
            
            return stack;
        }
    }
    return NULL;
}

void free_stack(size_t owner_tid) {
    if (n_slot == 0) return;

    for (size_t i = 0; i < MAX_STACK_SLOT; i++) {
        StackSlot* slot = &slots[i];
        if (slot->owner_tid == owner_tid) {
            slot->allocated = FALSE;
            n_slot--;
            return;
        }
    }
}
