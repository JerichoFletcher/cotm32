#pragma once

#include "context.h"
#include "int.h"

void k_yield(Context* ctx);
void k_putc(char c);
char k_getc(void);
void k_puts(const char* s, size_t len);
