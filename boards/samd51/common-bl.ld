/**
 *
 *
 *
 *
 *
 */
ENTRY(main)

SECTIONS
{
  .vectors ALIGN(0x2000) :
  {
    KEEP(*(.isr_vector))
  } > BOOTLOADER

  .data.fkb.header : {
    KEEP(*(.fkb.header))
  } > BOOTLOADER

  .text ALIGN(0x4) :
  {
    *(.text*)
    *(.rodata*)

    /* These are usually zero in this situation, eliminates warnings, though. */
    *(.glue_7)
    *(.glue_7t)
    *(.vfp11_veneer)
    *(.v4_bx)
    *(.dyn)
    *(.iplt)
    *(.rel.dyn)
    *(.rel.iplt)
  } > BOOTLOADER

  .ARM.extab ALIGN(0x4) :
  {
    *(.ARM.extab* .gnu.linkonce.armextab.*)
  } > BOOTLOADER

  __exidx_start = .;

  .ARM.exidx ALIGN(0x4) :
  {
    *(.ARM.exidx* .gnu.linkonce.armexidx.*)
  } > BOOTLOADER

  __exidx_end = .;

  __etext = .;
  _etext = .;

  .data.rtt : AT (__etext)
  {
    _srelocate = .;
    __data_start__ = .;
    *(SORT_BY_INIT_PRIORITY(.rtt.control))
    *(SORT_BY_INIT_PRIORITY(.rtt.buffers))
  } > RAM

  .data ALIGN(4) :
  {
    *(.data*)

    . = ALIGN(4);
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP(*(.preinit_array))
    PROVIDE_HIDDEN (__preinit_array_end = .);

    . = ALIGN(4);
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP(*(SORT(.init_array.*)))
    KEEP(*(.init_array))
    PROVIDE_HIDDEN (__init_array_end = .);

    . = ALIGN(4);
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP(*(SORT(.fini_array.*)))
    KEEP(*(.fini_array))
    PROVIDE_HIDDEN (__fini_array_end = .);

    . = ALIGN(4);
    __data_end__ = .;
    _erelocate = .;
  } > RAM

  .bss (NOLOAD) :
  {
    . = ALIGN(4);
    _szero = .;
    __bss_start__ = .;
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    __bss_end__ = .;
    _ezero = .;
  } > RAM

  .noinit (NOLOAD) :
  {
    . = ALIGN(4);
    __noinit_start__ = .;
    *(.noinit*)
    . = ALIGN(4);
    __noinit_end__ = .;
  } > RAM

  .heap (COPY):
  {
    __end__ = .;
    PROVIDE(end = .);
    *(.heap*)
    __heap_top = .;
  } > RAM

  .stack (COPY):
  {
    *(.stack*)
  } > RAM

  PROVIDE(__cm_app_vectors_ptr = ORIGIN(BOOTLOADER) + LENGTH(BOOTLOADER));

  __cm_ram_origin = ORIGIN(RAM);
  __cm_ram_end__ = ORIGIN(RAM) + LENGTH(RAM) - 1;
  __cm_stack_top = ORIGIN(RAM) + LENGTH(RAM);
  __cm_stack_max = __cm_stack_top - SIZEOF(.stack);

  ASSERT(__cm_stack_max >= __heap_top, "Region RAM overflowed with stack")
}
