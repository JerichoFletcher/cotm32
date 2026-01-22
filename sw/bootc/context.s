.global mret_to_context

# a0 -- Pointer to a context
mret_to_context:
    lw      t0, 128(a0)
    csrw    mepc, t0

    lw      t0, 132(a0)
    csrw    mstatus, t0

    lw      x1, 4(a0)
    lw      x2, 8(a0)
    lw      x3, 12(a0)
    lw      x4, 16(a0)
    lw      x5, 20(a0)
    lw      x6, 24(a0)
    lw      x7, 28(a0)
    lw      x8, 32(a0)
    lw      x9, 36(a0)
    
    lw      x11, 44(a0)
    lw      x12, 48(a0)
    lw      x13, 52(a0)
    lw      x14, 56(a0)
    lw      x15, 60(a0)
    lw      x16, 64(a0)
    lw      x17, 68(a0)
    lw      x18, 72(a0)
    lw      x19, 76(a0)
    lw      x20, 80(a0)
    lw      x21, 84(a0)
    lw      x22, 88(a0)
    lw      x23, 92(a0)
    lw      x24, 96(a0)
    lw      x25, 100(a0)
    lw      x26, 104(a0)
    lw      x27, 108(a0)
    lw      x28, 112(a0)
    lw      x29, 116(a0)
    lw      x30, 120(a0)
    lw      x31, 124(a0)
    
    lw      a0, 40(a0)
    mret
