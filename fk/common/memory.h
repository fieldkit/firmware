#pragma once

/**
 * Size of the bootloader.
 */
#define FK_MEMORY_BOOTLOADER_SIZE              (0x8000)

/**
 * Size of the bootloader we'd eventually like to use.
 */
#define FK_MEMORY_BOOTLOADER_SIZE_NEW          (FK_MEMORY_BOOTLOADER_SIZE * 2)

/**
 * Base address of the firmware that is running. This would be the
 * bootloader address.
 */
#define FK_MEMORY_FLASH_ADDRESS_RUNNING_BASE   (0x00000000)

/**
 * Base address of the core firmware that is running.
 */
#define FK_MEMORY_FLASH_ADDRESS_RUNNING_CORE   (FK_MEMORY_FLASH_ADDRESS_RUNNING_BASE + FK_MEMORY_BOOTLOADER_SIZE)

/**
 * Base address, in flash, of the other firmware we can optional swap
 * banks to use. Eventually we outgrow this.
 */
#define FK_MEMORY_FLASH_ADDRESS_UPGRADE_BASE   (0x00000000 + 0x80000)

/**
 * Address of the core firmware for the other bank.
 */
#define FK_MEMORY_FLASH_ADDRESS_UPGRADE_CORE   (FK_MEMORY_FLASH_ADDRESS_UPGRADE_BASE + FK_MEMORY_BOOTLOADER_SIZE)

/**
 * Maximum size of firmware. Notice that this is total maximum
 * supported and doesn't take into consideration the bank swaping.
 */
#define FK_MEMORY_FLASH_MAXIMUM_FIRMWARE_SIZE  (0x100000)

/**
 * Base address that QSPI memory is mapped onto.
 */
#define FK_MEMORY_QSPI_BASE                    (0x04000000)

/**
 * Size of QSPI memory in bytes.
 */
#define FK_MEMORY_QSPI_SIZE                    (0x800000)

/**
 * Base address of the upgrade bank in QSPI memory. Banks are
 * intentionally located at the end of the QSPI memory.
 */
#define FK_MEMORY_QSPI_ADDRESS_UPGRADE_BASE    (FK_MEMORY_QSPI_BASE + FK_MEMORY_QSPI_SIZE - FK_MEMORY_FLASH_MAXIMUM_FIRMWARE_SIZE)

/**
 * Base address of the upgrade bank's core firmware in QSPI
 * memory. Banks are intentionally located at the end of the QSPI
 * memory.
 */
#define FK_MEMORY_QSPI_ADDRESS_UPGRADE_CORE    (FK_MEMORY_QSPI_ADDRESS_UPGRADE_BASE + FK_MEMORY_BOOTLOADER_SIZE_NEW)

/**
 * Base address of the failsafe bank in QSPI memory. Banks are
 * intentionally located at the end of the QSPI memory.
 */
#define FK_MEMORY_QSPI_ADDRESS_FAILSAFE_BASE   (FK_MEMORY_QSPI_BASE + FK_MEMORY_QSPI_SIZE - FK_MEMORY_FLASH_MAXIMUM_FIRMWARE_SIZE - FK_MEMORY_FLASH_MAXIMUM_FIRMWARE_SIZE)

/**
 * Base address of the failsafe bank's core firmware in QSPI
 * memory. Banks are intentionally located at the end of the QSPI
 * memory.
 */
#define FK_MEMORY_QSPI_ADDRESS_FAILSAFE_CORE   (FK_MEMORY_QSPI_ADDRESS_FAILSAFE_BASE + FK_MEMORY_BOOTLOADER_SIZE_NEW)

/**
 * Maximum size of the vector table at the start of the
 * bootloader. This is used to calculate an offset into memory to
 * check for the fkb_header_t struct.
 *
 * Also note that this should be within NetworkBufferSize as well so
 * that first-page erases nuke this as well.
 */
#define FK_MEMORY_MAXIMUM_VTOR_SIZE            (0x40)
