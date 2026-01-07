ENTRY(_start)

MEMORY {
  IMEM (rx) : ORIGIN = 0x00000000, LENGTH = 1K
  ROM (rx)  : ORIGIN = 0x00010000, LENGTH = 1K
  DMEM (rwx): ORIGIN = 0x10000000, LENGTH = 4K
}

SECTIONS {
  . = ORIGIN(IMEM);
  .text   : { *(.text*) } > IMEM

  . = ORIGIN(ROM);
  .rodata : { *(.rodata*) } > ROM

  _estack = ORIGIN(DMEM) + LENGTH(DMEM);
}