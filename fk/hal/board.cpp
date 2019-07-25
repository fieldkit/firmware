#include "board.h"

#if defined(ARDUINO)
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#endif

namespace fk {

static Board board;

Board *get_board() {
    return &board;
}

#if defined(ARDUINO)

const uint8_t spi_pins[] = {
    PIN_SPI_MISO,  PIN_SPI_MOSI,  PIN_SPI_SCK,
    PIN_SPI1_MISO, PIN_SPI1_MOSI, PIN_SPI1_SCK,
};

const uint8_t i2c_pins[] = {
    PIN_WIRE_SDA,  PIN_WIRE_SCL,
    PIN_WIRE1_SDA, PIN_WIRE1_SCL,
};

const uint8_t qspi_pins[] = {
    PIN_QSPI_SCK,
    PIN_QSPI_CS,
    PIN_QSPI_IO0,
    PIN_QSPI_IO1,
    PIN_QSPI_IO2,
    PIN_QSPI_IO3,
};

const uint8_t radio_spi_cs_pins[] = {
    WINC1500_CS,
};

const uint8_t core_spi_cs_pins[] = {
    SPI_FLASH_CS_BANK_1,
    SPI_FLASH_CS_BANK_2,
    SPI_FLASH_CS_BANK_3,
    SPI_FLASH_CS_BANK_4,
    PIN_SD_CS,
};

const uint8_t power_pins[] = {
    WINC1500_POWER,
    GPS_POWER,
};

void Board::initialize() {
    pinMode(WINC1500_POWER, OUTPUT);
    pinMode(WINC1500_CS, OUTPUT);
    pinMode(WINC1500_IRQ, INPUT);
    pinMode(WINC1500_RESET, OUTPUT);

    pinMode(GPS_POWER, OUTPUT);

    for (auto pin : radio_spi_cs_pins) {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, HIGH);
    }

    for (auto pin : core_spi_cs_pins) {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, HIGH);
    }

    disable_everything();
}

void Board::disable_everything() {
    spi_flash().end();
    spi_radio().end();
    spi_module().end();
    i2c_core().end();
    i2c_radio().end();
    i2c_module().end();
    disable_gps();
    disable_wifi();
}

void Board::enable_everything() {
    enable_gps();
    enable_wifi();
    Wire1.begin();
}

void Board::disable_gps() {
    digitalWrite(GPS_POWER, LOW);
}

void Board::enable_gps() {
    digitalWrite(GPS_POWER, HIGH);
}

void Board::disable_wifi() {
    digitalWrite(WINC1500_POWER, LOW);
}

void Board::enable_wifi() {
    digitalWrite(WINC1500_POWER, HIGH);
    SPI1.begin();
}

SpiWrapper Board::spi_flash() {
    return { &SPI };
}

SpiWrapper Board::spi_sd() {
    return { &SPI2 };
}

SpiWrapper Board::spi_radio() {
    return { &SPI1 };
}

SpiWrapper Board::spi_module() {
    return { nullptr };
}

TwoWireWrapper Board::i2c_core() {
    return { &Wire };
}

TwoWireWrapper Board::i2c_radio() {
    return { &Wire1 };
}

TwoWireWrapper Board::i2c_module() {
    return { &Wire2 };
}

SerialWrapper Board::gps_serial() {
    return { &Serial1 };
}

void SpiWrapper::begin() {
    if (ptr_ == nullptr) return;

    reinterpret_cast<SPIClass*>(ptr_)->begin();
}

void SpiWrapper::end() {
    if (ptr_ == nullptr) return;

    reinterpret_cast<SPIClass*>(ptr_)->end();

    if (ptr_ == &SPI) {
        pinMode(PIN_SPI_MISO, INPUT);
        pinMode(PIN_SPI_MOSI, INPUT);
        pinMode(PIN_SPI_SCK, INPUT);
    }
    else if (ptr_ == &SPI1) {
        pinMode(PIN_SPI1_MISO, INPUT);
        pinMode(PIN_SPI1_MOSI, INPUT);
        pinMode(PIN_SPI1_SCK, INPUT);
    }
    else if (ptr_ == &SPI2) {
        pinMode(PIN_SPI2_MISO, INPUT);
        pinMode(PIN_SPI2_MOSI, INPUT);
        pinMode(PIN_SPI2_SCK, INPUT);
    }
}

void TwoWireWrapper::begin() {
    if (ptr_ == nullptr) return;

    reinterpret_cast<TwoWire*>(ptr_)->begin();
}

void TwoWireWrapper::end() {
    if (ptr_ == nullptr) return;

    reinterpret_cast<TwoWire*>(ptr_)->end();

    if (ptr_ == &Wire) {
        pinMode(PIN_WIRE_SDA, INPUT);
        pinMode(PIN_WIRE_SCL, INPUT);
    }
    else if (ptr_ == &Wire1) {
        pinMode(PIN_WIRE1_SDA, INPUT);
        pinMode(PIN_WIRE1_SCL, INPUT);
    }
    else if (ptr_ == &Wire2) {
        pinMode(PIN_WIRE2_SDA, INPUT);
        pinMode(PIN_WIRE2_SCL, INPUT);
    }
}

bool SerialWrapper::begin(uint32_t baud) {
    reinterpret_cast<Uart*>(ptr_)->begin(baud);
    return true;
}

bool SerialWrapper::end() {
    reinterpret_cast<Uart*>(ptr_)->end();
    return true;
}

int32_t SerialWrapper::available() {
    return reinterpret_cast<Uart*>(ptr_)->available();
}

int8_t SerialWrapper::read() {
    return reinterpret_cast<Uart*>(ptr_)->read();
}


#endif

}
