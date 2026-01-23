#pragma once

/// @brief Various codes for system calls.
typedef enum SyscallCode {
    SyscallCode_EXIT    = 1,
    SyscallCode_YIELD   = 2,
    SyscallCode_PUTC    = 32,
    SyscallCode_GETC    = 33,
    SyscallCode_PUTS    = 34,
} SyscallCode;
