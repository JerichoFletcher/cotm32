ENTRY(_start)

MEMORY {
  BOOTROM (rx)  : ORIGIN = 0x00000000, LENGTH = 4K
  CLINT (rw)    : ORIGIN = 0x02000000, LENGTH = 64K
  RAM (rwx)     : ORIGIN = 0x80000000, LENGTH = 4K
}

SECTIONS {
  . = ORIGIN(BOOTROM);
  .text   : {
    *(.text.init)
    *(.text*)
  } > BOOTROM

  . = ALIGN(4);
  .rodata : {
    *(.rodata*)
  } > BOOTROM
  
  _etext = .;

  _estack = ORIGIN(RAM) + LENGTH(RAM);
}