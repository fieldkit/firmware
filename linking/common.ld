/**
 *
 *
 *
 *
 *
 */
ENTRY(Reset_Handler)

SECTIONS
{
  .data.fkb.header :
  {
    KEEP(*(.fkb.header))
		. = ALIGN(0x4000);
  } > FLASH

	.text :
	{
		__text_start__ = .;

    /**
     * IRQs     Align
     * 0-16     32-word
     * 17-48	  64-word
     * 49-112   128-word
     * 113-240	256-word
     */
		KEEP(*(.isr_vector))
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
	} > FLASH

	.ARM.extab :
	{
		*(.ARM.extab* .gnu.linkonce.armextab.*)
	} > FLASH

	__exidx_start = .;
	.ARM.exidx :
	{
		*(.ARM.exidx* .gnu.linkonce.armexidx.*)
	} > FLASH
  __exidx_end = .;

  __etext_global = .;

  .data.fkb.launch : AT(__etext_global)
  {
    KEEP(*(.fkb.launch))
  } > RAM

  .data.rtt :
  {
    *(SORT_BY_INIT_PRIORITY(.rtt.control))
    *(SORT_BY_INIT_PRIORITY(.rtt.buffers))
  } > RAM

  __etext = __etext_global + SIZEOF(.data.rtt) + SIZEOF(.data.fkb.launch);

  .data : AT(__etext)
	{
		__data_start__ = .;
		*(vtable)
		*(.data*)

		. = ALIGN(4);
		/* preinit data */
		PROVIDE_HIDDEN (__preinit_array_start = .);
		KEEP(*(.preinit_array))
		PROVIDE_HIDDEN (__preinit_array_end = .);

		. = ALIGN(4);
		/* init data */
		PROVIDE_HIDDEN (__init_array_start = .);
		KEEP(*(SORT(.init_array.*)))
		KEEP(*(.init_array))
		PROVIDE_HIDDEN (__init_array_end = .);

		. = ALIGN(4);
		/* finit data */
		PROVIDE_HIDDEN (__fini_array_start = .);
		KEEP(*(SORT(.fini_array.*)))
		KEEP(*(.fini_array))
		PROVIDE_HIDDEN (__fini_array_end = .);

		KEEP(*(.jcr*))
		. = ALIGN(16);
		__data_end__ = .;

	} > RAM

  .bss :
  {
    . = ALIGN(4);
    __bss_start__ = .;
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    __bss_end__ = .;
  } > RAM

  .got :
  {
    *(.got*)
  } > RAM

	.noinit :
	{
		. = ALIGN(4);
		__noinit_start__ = .;
		*(.noinit*)
		*(COMMON)
		. = ALIGN(4);
		__noinit_end__ = .;
	} > RAM

	.heap (COPY):
	{
		__end__ = .;
		PROVIDE(end = .);
		*(.heap*)
		__HeapLimit = .;
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

	__ram_end__ = ORIGIN(RAM) + LENGTH(RAM) - 1;

	/* Check if data + heap + stack exceeds RAM limit */
	ASSERT(__StackLimit >= __HeapLimit, "region RAM overflowed with stack")
}
