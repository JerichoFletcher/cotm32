#pragma once

#include "context.h"
#include "int.h"

typedef enum SyscallStatus {
    SyscallStatus_DONE,
    SyscallStatus_BLOCKED,
} SyscallStatus;

SyscallStatus k_exit(Context* ctx);
SyscallStatus k_yield(Context* ctx);
SyscallStatus k_putc(char c);
SyscallStatus k_getc(Context* ctx, char* out_c);
SyscallStatus k_puts(const char* s, size_t len);
