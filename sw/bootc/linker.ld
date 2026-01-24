ENTRY(_start)

MEMORY {
  IMEM (rx)  : ORIGIN = 0x00000000, LENGTH = 8K
  CLINT (rw)    : ORIGIN = 0x02000000, LENGTH = 64K
  UART (rw)     : ORIGIN = 0x10000000, LENGTH = 256
  RAM (rwx)     : ORIGIN = 0x80000000, LENGTH = 64K
}

SECTIONS {
  . = ORIGIN(IMEM);
  .text   : {
    *(.text.init)
    *(.text*)
  } > IMEM

  .rodata : {
    *(.rodata*)
  } > IMEM
  
  _etext = .;

  . = ORIGIN(RAM);
  .data   : {
    *(.data*)
  } > RAM

  __bss_start = .;
  .bss    : {
    *(.bss*)
    *(.sbss*)
    *(COMMON)
  } > RAM
  __bss_end = .;
  stackalloc_start = .;

  _estack = ORIGIN(RAM) + LENGTH(RAM);
}