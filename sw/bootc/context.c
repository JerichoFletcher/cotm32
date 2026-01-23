#include "context.h"

void copy_context(Context* to, Context* from) {
    for (size_t i = 0; i < NUM_REGS; i++) {
        to->regs[i] = from->regs[i];
    }
    to->mstatus = from->mstatus;
    to->mie = from->mie;
    to->pc = from->pc;
}
