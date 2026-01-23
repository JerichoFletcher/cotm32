#pragma once

/// @brief Stalls execution forever. 
__attribute__((noreturn)) static inline void panic(void) { for (;;); }
