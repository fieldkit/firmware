/**
 *
 *
 *
 *
 *
 */
/* If I leave this out, nothing ends up in the file, even with KEEP. I think if
 * you're missing any .text or executable code then everything is dropped. */
ENTRY(main)

SECTIONS
{
  .shim_vectors ALIGN(0x2000) :
  {
    KEEP(*(.shim_isr_vector))
  } > FLASH

  .text.shim ALIGN(0x4) :
  {
    KEEP(*(.shim_text))
  } > FLASH

  .text ALIGN(0x4) :
  {
    KEEP(*(.text))
    *(.rodata*)
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

  __etext_rtt = .;

  .rtt : AT(__etext_rtt)
  {
  } > RAM

  __etext = __etext_rtt + SIZEOF(.rtt);

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

  __cm_shim_stack_top = ORIGIN(RAM) + LENGTH(RAM);
}
