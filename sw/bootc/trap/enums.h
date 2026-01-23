#pragma once

/// @brief The value of `mcause` when entering a trap.
typedef enum TrapCause {
    TrapCause_INST_ADDR_MISALIGNED = 0,
    TrapCause_INST_ACCESS_FAULT = 1,
    TrapCause_ILLEGAL_INST = 2,
    TrapCause_BREAKPOINT = 3,
    TrapCause_LOAD_ADDR_MISALIGNED = 4,
    TrapCause_LOAD_ACCESS_FAULT = 5,
    TrapCause_STORE_ADDR_MISALIGNED = 6,
    TrapCause_STORE_ACCESS_FAULT = 7,
    TrapCause_ECALL_U = 8,
    TrapCause_ECALL_M = 11,

    TrapCause_INTERR_M_SOFTWARE = 3 | 0x80000000,
    TrapCause_INTERR_M_TIMER = 7 | 0x80000000,
    TrapCause_INTERR_M_EXTERNAL = 11 | 0x80000000,
} TrapCause;

/// @brief Identifiers for exceptions.
typedef enum Exception {
    Exception_INST_ADDR_MISALIGNED = 0,
    Exception_INST_ACCESS_FAULT = 1,
    Exception_ILLEGAL_INST = 2,
    Exception_BREAKPOINT = 3,
    Exception_LOAD_ADDR_MISALIGNED = 4,
    Exception_LOAD_ACCESS_FAULT = 5,
    Exception_STORE_ADDR_MISALIGNED = 6,
    Exception_STORE_ACCESS_FAULT = 7,
    Exception_ECALL_U = 8,
    Exception_ECALL_M = 11,
} Exception;

/// @brief Identifiers for interrupts.
typedef enum Interrupt {
    Interrupt_M_SOFTWARE = 3,
    Interrupt_M_TIMER = 7,
    Interrupt_M_EXTERNAL = 11,
} Interrupt;
    