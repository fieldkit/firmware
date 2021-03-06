#include <Arduino.h>
#include "wiring_private.h"

#include "bl.h"

enum {
    SFLASH_CMD_READ = 0x03,      // Single Read
    SFLASH_CMD_QUAD_READ = 0x6B, // 1 line address, 4 line data

    SFLASH_CMD_READ_JEDEC_ID = 0x9f,

    SFLASH_CMD_PAGE_PROGRAM = 0x02,
    SFLASH_CMD_QUAD_PAGE_PROGRAM = 0x32, // 1 line address, 4 line data

    SFLASH_CMD_READ_STATUS = 0x05,
    SFLASH_CMD_READ_STATUS2 = 0x35,

    SFLASH_CMD_WRITE_STATUS = 0x01,
    SFLASH_CMD_WRITE_STATUS2 = 0x31,

    SFLASH_CMD_ENABLE_RESET = 0x66,
    SFLASH_CMD_RESET = 0x99,

    SFLASH_CMD_WRITE_ENABLE = 0x06,
    SFLASH_CMD_WRITE_DISABLE = 0x04,

    SFLASH_CMD_ERASE_SECTOR = 0x20,
    SFLASH_CMD_ERASE_BLOCK = 0xD8,
    SFLASH_CMD_ERASE_CHIP = 0xC7,
};

// Copied from `void Adafruit_FlashTransport_QSPI`
static void samd_peripherals_disable_and_clear_cache() {
    CMCC->CTRL.bit.CEN = 0;
    while ( CMCC->SR.bit.CSTS ) { }
    CMCC->MAINT0.bit.INVALL = 1;
}

// Copied from `void Adafruit_FlashTransport_QSPI`
static void samd_peripherals_enable_cache() {
    CMCC->CTRL.bit.CEN = 1;
}

// Copied from `static bool _run_instruction(uint8_t command, uint32_t iframe, uint32_t addr, uint8_t *buffer, uint32_t size)`
static bool _run_instruction(uint8_t command, uint32_t iframe, uint32_t addr, uint8_t *buffer, uint32_t size) {
    samd_peripherals_disable_and_clear_cache();

    uint8_t *qspi_mem = (uint8_t *)QSPI_AHB;
    if (addr)
        qspi_mem += addr;

    QSPI->INSTRCTRL.bit.INSTR = command;
    QSPI->INSTRADDR.reg = addr;

    QSPI->INSTRFRAME.reg = iframe;

    // Dummy read of INSTRFRAME needed to synchronize.
    // See Instruction Transmission Flow Diagram, figure 37.9, page 995
    // and Example 4, page 998, section 37.6.8.5.
    (volatile uint32_t) QSPI->INSTRFRAME.reg;

    if (buffer && size) {
        uint32_t const tfr_type = iframe & QSPI_INSTRFRAME_TFRTYPE_Msk;

        if ((tfr_type == QSPI_INSTRFRAME_TFRTYPE_READ) || (tfr_type == QSPI_INSTRFRAME_TFRTYPE_READMEMORY)) {
            memcpy(buffer, qspi_mem, size);
        } else {
            memcpy(qspi_mem, buffer, size);
        }
    }

    __asm__ volatile("dsb");
    __asm__ volatile("isb");

    QSPI->CTRLA.reg = QSPI_CTRLA_ENABLE | QSPI_CTRLA_LASTXFER;

    while (!QSPI->INTFLAG.bit.INSTREND) {
    }
    QSPI->INTFLAG.bit.INSTREND = 1;

    samd_peripherals_enable_cache();

    return true;
}

extern "C" {

int32_t bl_qspi_read_memory(uint32_t addr, uint8_t *data, uint32_t len) {
    // Copied from `bool Adafruit_FlashTransport_QSPI::readMemory(uint32_t addr, uint8_t *data, uint32_t len)`
    uint32_t iframe = QSPI_INSTRFRAME_WIDTH_QUAD_OUTPUT | QSPI_INSTRFRAME_ADDRLEN_24BITS |
                      QSPI_INSTRFRAME_TFRTYPE_READMEMORY | QSPI_INSTRFRAME_INSTREN | QSPI_INSTRFRAME_ADDREN |
                      QSPI_INSTRFRAME_DATAEN |
                      /*QSPI_INSTRFRAME_CRMODE |*/ QSPI_INSTRFRAME_DUMMYLEN(8);

    return _run_instruction(SFLASH_CMD_QUAD_READ, iframe, addr, data, len);
}

int32_t bl_qspi_initialize() {
    // Copied from `void Adafruit_FlashTransport_QSPI::begin(void)`
    MCLK->AHBMASK.reg |= MCLK_AHBMASK_QSPI;
    MCLK->AHBMASK.reg |= MCLK_AHBMASK_QSPI_2X;
    MCLK->APBCMASK.reg |= MCLK_APBCMASK_QSPI;

    // set all pins to QSPI periph
    pinPeripheral(PIN_QSPI_SCK, PIO_COM);
    pinPeripheral(PIN_QSPI_CS, PIO_COM);
    pinPeripheral(PIN_QSPI_IO0, PIO_COM);
    pinPeripheral(PIN_QSPI_IO1, PIO_COM);
    pinPeripheral(PIN_QSPI_IO2, PIO_COM);
    pinPeripheral(PIN_QSPI_IO3, PIO_COM);

    QSPI->CTRLA.bit.SWRST = 1;

    delay(1); // no syncbusy reg.. do we need this? Probably not

    QSPI->CTRLB.reg =
        QSPI_CTRLB_MODE_MEMORY | QSPI_CTRLB_CSMODE_NORELOAD | QSPI_CTRLB_DATALEN_8BITS | QSPI_CTRLB_CSMODE_LASTXFER;

    QSPI->BAUD.reg = QSPI_BAUD_BAUD(VARIANT_MCK / 4000000UL); // start with low 4Mhz, Mode 0

    QSPI->CTRLA.bit.ENABLE = 1;

    return bl_qspi_read_memory(0, nullptr, 0);
}

}
