/**
 *
 *
 *
 */
MEMORY
{
  MODULE (rx) : ORIGIN = 0x00000000, LENGTH = 0x00008000
}

ENTRY(fkmodule)

SECTIONS
{
  .data.fkb.header : {
    KEEP(*(.fkb.header))
    . = ALIGN(0x400);
  } > MODULE

  .text ALIGN(0x4) :
  {
    *(.text*)
    *(.rodata*)
  } > MODULE

  .rel.plt :
  {
    __got_start__ = .;
    *(.rel.plt*)
    . = ALIGN(4);
    __got_end__ = .;
  } > MODULE

  .got :
  {
    __got_start__ = .;
    *(.got*)
    . = ALIGN(4);
    __got_end__ = .;
  } > MODULE

  .data :
  {
    __data_start__ = .;
    *(.data*)
    . = ALIGN(4);
    __data_end__ = .;
  } > MODULE

  .bss :
  {
    __bss_start__ = .;
    *(.bss*)
    . = ALIGN(4);
    __bss_end__ = .;
  } > MODULE
}
