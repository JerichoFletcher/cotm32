#include "kernel/stack.h"
#include "bool.h"

typedef struct StackSlot {
    StackDescriptor stack;
    bool_t allocated;
} StackSlot;

StackSlot slots[MAX_STACK_SLOT] = {0};
size_t n_slot = 0;

StackDescriptor* alloc_stack(void) {
    if (n_slot == MAX_STACK_SLOT) return NULL;
    
    for (size_t i = 0; i < MAX_STACK_SLOT; i++) {
        StackSlot* slot = &slots[i];
        if (!slot->allocated) {
            slot->allocated = TRUE;
            
            StackDescriptor* stack = &slot->stack;
            stack->base = (char*)STACK_SPACE_START + i * STACK_SIZE;
            stack->size = STACK_SIZE;
            
            n_slot++;
            return stack;
        }
    }
    return NULL;
}

void free_stack(StackDescriptor* stack) {
    if (n_slot == 0) return;

    for (size_t i = 0; i < MAX_STACK_SLOT; i++) {
        StackSlot* slot = &slots[i];
        if (stack == &slot->stack) {
            slot->allocated = FALSE;
            n_slot--;
            return;
        }
    }
}
