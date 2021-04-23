/**
 *
 *
 *
 *
 *
 */
MEMORY
{
  FLASH (rx) : ORIGIN = 0x00000000 + 0x4000, LENGTH = 0x00040000 - 0x4000
  RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 0x00008000
}

INCLUDE common.ld
