/**
 *
 *
 *
 */
MEMORY
{
  FLASH (rx) : ORIGIN = 0x04000000, LENGTH = 0x100000
  RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 0x00040000
}

INCLUDE common-main.ld
