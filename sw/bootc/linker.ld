ENTRY(_start)

MEMORY {
  IMEM (rx)  : ORIGIN = 0x00000000, LENGTH = 4K
  CLINT (rw)    : ORIGIN = 0x02000000, LENGTH = 64K
  UART (rw)     : ORIGIN = 0x10000000, LENGTH = 256
  RAM (rwx)     : ORIGIN = 0x80000000, LENGTH = 4K
}

SECTIONS {
  . = ORIGIN(IMEM);
  .text   : {
    . = ALIGN(4);
    *(.text.init)
    . = ALIGN(4);
    *(.text*)
  } > IMEM

  . = ALIGN(4);
  .rodata : {
    *(.rodata*)
  } > IMEM
  
  _etext = .;

  _estack = ORIGIN(RAM) + LENGTH(RAM);
}