#include "sc16is740.h"

#if defined(ARDUINO)

namespace fk {

#include <Arduino.h>

constexpr uint8_t Sc16iS740Address = 0x9A >> 1;

static const uint8_t OPTIONS_8N1 = 0b000011;
static const uint8_t OPTIONS_8E1 = 0b011011;
static const uint8_t OPTIONS_8O1 = 0b001011;

static const uint8_t OPTIONS_8N2 = 0b000111;
static const uint8_t OPTIONS_8E2 = 0b011111;
static const uint8_t OPTIONS_8O2 = 0b001111;

static const uint8_t OPTIONS_7N1 = 0b000010;
static const uint8_t OPTIONS_7E1 = 0b011010;
static const uint8_t OPTIONS_7O1 = 0b001010;

static const uint8_t OPTIONS_7N2 = 0b000110;
static const uint8_t OPTIONS_7E2 = 0b011110;
static const uint8_t OPTIONS_7O2 = 0b001110;

static const uint8_t RHR_THR_REG = 0x00;
static const uint8_t IEF_REG = 0x01;
static const uint8_t FCR_IIR_REG = 0x02;
static const uint8_t LCR_REG = 0x03;
static const uint8_t MCR_REG = 0x04;
static const uint8_t LSR_REG = 0x05;
static const uint8_t MSR_REG = 0x06;
static const uint8_t SPR_REG = 0x07;
static const uint8_t TXLVL_REG = 0x08;
static const uint8_t RXLVL_REG = 0x09;
static const uint8_t IODIR_REG = 0x0a;
static const uint8_t IOSTATE_REG = 0x0b;
static const uint8_t IOINTENA_REG = 0x0c;
static const uint8_t IOCONTROL_REG = 0x0e;
static const uint8_t EFCR_REG = 0x0f;

// Special register block
static const uint8_t LCR_SPECIAL_START = 0x80;
static const uint8_t LCR_SPECIAL_END = 0xbf;
static const uint8_t DLL_REG = 0x00;
static const uint8_t DLH_REG = 0x01;

// Enhanced register set
static const uint8_t EFR_REG = 0x02;
static const uint8_t XON1_REG = 0x04;
static const uint8_t XON2_REG = 0x05;
static const uint8_t XOFF1_REG = 0x06;
static const uint8_t XOFF2_REG = 0x07;

SC16IS740::SC16IS740(TwoWireWrapper &bus) : bus_(&bus) {
}

bool SC16IS740::begin() {
    bus_->begin();

    auto oscillator_hz  = 3686400;
    auto baud_rate = 57600;
    auto div = oscillator_hz / (baud_rate * 16);

    if (!write_register(LCR_REG, LCR_SPECIAL_START)) return false;
    if (!write_register(DLL_REG, div & 0xff)) return false;
    if (!write_register(DLH_REG, div >> 8)) return false;
    if (!write_register(LCR_REG, LCR_SPECIAL_END)) return false;
    if (!write_register(LCR_REG, OPTIONS_8N1 & 0x3f)) return false;
    if (!write_register(FCR_IIR_REG, 0x07)) return false;

    return true;
}

int32_t SC16IS740::available_for_read() {
    uint8_t available = 0;
    if (!read_register(RXLVL_REG, available)) {
        return -1;
    }
    return available;
}

int32_t SC16IS740::available_for_write() {
    uint8_t available = 0;
    if (!read_register(TXLVL_REG, available)) {
        return -1;
    }
    return available;
}

bool SC16IS740::read_fifo(uint8_t *buffer, size_t size) {
    uint8_t setup[1] = {
        (uint8_t)(RHR_THR_REG << 3)
    };

    if (!I2C_CHECK(bus_->write(Sc16iS740Address, setup, sizeof(setup)))) {
        return false;
    }

    if (!I2C_CHECK(bus_->read(Sc16iS740Address, buffer, size))) {
        return false;
    }

    return true;
}

bool SC16IS740::write_fifo(uint8_t const *buffer, size_t size) {
    uint8_t data[1 + size];
    data[0] = RHR_THR_REG << 3;
    memcpy(data + 1, buffer, size);

    if (!I2C_CHECK(bus_->write(Sc16iS740Address, data, sizeof(data)))) {
        return false;
    }

    return true;
}

bool SC16IS740::write(const char *line) {
    auto nwrite = available_for_write();
    if (nwrite < 0) {
        return false;
    }
    auto len = (int32_t)strlen(line);
    FK_ASSERT(len <= nwrite);
    return write_fifo((uint8_t *)line, len);
}

bool SC16IS740::write_register(uint8_t reg, uint8_t value) {
    uint8_t buffer[2] = {
        (uint8_t)(reg << 3),
        value,
    };

    if (!I2C_CHECK(bus_->write(Sc16iS740Address, buffer, sizeof(buffer)))) {
        return false;
    }

    return true;
}

bool SC16IS740::read_register(uint8_t reg, uint8_t &value) {
    uint8_t buffer[1] = {
        (uint8_t)(reg << 3)
    };

    if (!I2C_CHECK(bus_->write(Sc16iS740Address, buffer, sizeof(buffer)))) {
        return false;
    }

    uint8_t data[1];

    if (!I2C_CHECK(bus_->read(Sc16iS740Address, data, sizeof(data)))) {
        return false;
    }

    value = data[0];

    return true;
}

}

#endif
