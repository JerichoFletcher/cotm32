ENTRY(_start)

MEMORY {
  ROM (rx)  : ORIGIN = 0x00000000, LENGTH = 1K
  RAM (rwx) : ORIGIN = 0x10000000, LENGTH = 4K
}

SECTIONS {
  . = ORIGIN(ROM);
  .text   : { *(.text*) } > ROM
  .rodata : { *(.rodata*) } > ROM

  _estack = ORIGIN(RAM) + LENGTH(RAM);
}