#pragma once

#include "priv/enums.h"
#include "csr.h"
#include "bool.h"
#include "int.h"

static inline void prepare_enter(void (*f)(void), PrivMode next_priv, bool_t mie) {
    assign_mpie(mie);
    assign_mpp(next_priv);
    assign_mepc(f);
}
