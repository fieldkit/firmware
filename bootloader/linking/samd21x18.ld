/**
 *
 *
 *
 *
 *
 */
MEMORY
{
  FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 0x00004000
  RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 0x00008000
}

INCLUDE common.ld
