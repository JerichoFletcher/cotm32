#include "sys/enums.h"
#include "context.h"

/// @brief Dispatches a system call based on the given context.
/// @brief System calls will read arguments from and write return values to the context.
/// @param ctx The execution context for the system call.
void dispatch_syscall(Context* ctx);
