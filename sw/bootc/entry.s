.global _start
.extern _estack

.section .text.init
_start:
    la      sp, _estack
    call    main
    j       .
