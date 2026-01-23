#pragma once

/// @brief Privilege modes. Determines the level of access that a task has.
typedef enum PrivMode {
    /// @brief User mode. The lowest privilege level that grants limited access.
    PrivMode_U = 0b00,
    /// @brief Machine mode. The highest privilege level at which everything is accessible,
    /// @brief including privileged instructions, memory, and peripherals.
    PrivMode_M = 0b11,
} PrivMode;
