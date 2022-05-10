/**
 *
 *
 *
 */
ENTRY(Reset_Handler)

SECTIONS
{
  .data.fkb.header : {
    KEEP(*(.fkb.header))
    . = ALIGN(0x400);
  } > FLASH

  .text :
  {
    __text_start__ = .;

    /**
     * IRQs      Align
     * 0-16      32-word
     * 17-48     64-word
     * 49-112   128-word
     * 113-240	256-word
     */
    _sfixed = .;
    KEEP(*(.isr_vector))
    KEEP(*(.vectors))
    *(.text*)

    KEEP(*(.init))
    KEEP(*(.fini))

    /* .ctors */
    *crtbegin.o(.ctors)
    *crtbegin?.o(.ctors)
    *(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
    *(SORT(.ctors.*))
    *(.ctors)

    /* .dtors */
    *crtbegin.o(.dtors)
    *crtbegin?.o(.dtors)
    *(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
    *(SORT(.dtors.*))
    *(.dtors)

    *(.rodata*)

    KEEP(*(.eh_frame*))

    . = . + 28;
  } > FLASH

  .ARM.extab ALIGN(0x4) :
  {
    *(.ARM.extab* .gnu.linkonce.armextab.*)
  } > FLASH

  __exidx_start = .;

  .ARM.exidx ALIGN(0x4) :
  {
    *(.ARM.exidx* .gnu.linkonce.armexidx.*)
  } > FLASH

  __exidx_end = .;

  __etext = .;
  _etext = .;

  .data.rtt : AT (__etext)
  {
    __data_start__ = .;
    *(SORT_BY_INIT_PRIORITY(.rtt.control))
    *(SORT_BY_INIT_PRIORITY(.rtt.buffers))
  } > RAM

  .data ALIGN(4) :
  {
    _srelocate = .;
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
    __bss_start__ = .;
    _szero = .;
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    __bss_end__ = .;
    _ezero = .;
  } > RAM

  .got :
  {
    *(.got*)
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
    __heap_start__ = .;
    PROVIDE(end = .);
    *(.heap*)
    __heap_end__ = .;
  } > RAM

  /* .stack_dummy section doesn't contains any symbols. It is only
  * used for linker to calculate size of stack sections, and assign
  * values to stack symbols later */
  .stack_dummy (COPY):
  {
    *(.stack*)
  } > RAM

  /* Set stack top to end of RAM, and stack limit move down by
  * size of stack_dummy section */
  __StackTop = ORIGIN(RAM) + LENGTH(RAM);
  __StackLimit = __StackTop - SIZEOF(.stack_dummy);

  PROVIDE(__stack = __StackTop);

  __cm_ram_origin = ORIGIN(RAM);
  __cm_ram_origin__ = ORIGIN(RAM);
  __cm_ram_end__ = ORIGIN(RAM) + LENGTH(RAM) - 1;

  __ram_end__ = ORIGIN(RAM) + LENGTH(RAM) - 1;
  _estack = ORIGIN(RAM) + LENGTH(RAM) - 1;

  /* Check if data + heap + stack exceeds RAM limit */
  ASSERT(__StackLimit >= __heap_end__, "Region RAM overflowed with stack")
}
