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
  } > FLASH

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

  __etext_global = .;

  .data.fkb.launch (NOLOAD): AT(__etext_global)
  {
    KEEP(*(.fkb.launch))
  } > RAM

  .data.rtt :
  {
    *(SORT_BY_INIT_PRIORITY(.rtt.control))
    *(SORT_BY_INIT_PRIORITY(.rtt.buffers))
  } > RAM

  __etext = __etext_global + SIZEOF(.data.rtt) + SIZEOF(.data.fkb.launch);

  .data : AT (__etext)
  {
    __data_start__ = .;
    *(.data*)
    . = ALIGN(4);
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

  PROVIDE(__cm_app_vectors_ptr = ORIGIN(FLASH) + LENGTH(FLASH));

  __cm_ram_origin = ORIGIN(RAM);
  __cm_ram_end = ORIGIN(RAM) + LENGTH(RAM);
  __cm_stack_top = __cm_ram_end;
  __cm_stack_max = __cm_stack_top - SIZEOF(.stack);

	ASSERT(__cm_stack_max >= __heap_top, "Region RAM overflowed with stack")
}
