#pragma once

#include "priv/enums.h"
#include "bool.h"
#include "int.h"

__attribute__((noreturn))
void enter(void* f, PrivMode next_priv, bool_t mie);
