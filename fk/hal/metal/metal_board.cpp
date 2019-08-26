#include "hal/board.h"

#if defined(ARDUINO)

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>

namespace fk {

FK_DECLARE_LOGGER("board");

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

bool Board::initialize() {
    pinMode(WINC1500_POWER, OUTPUT);
    pinMode(WINC1500_CS, OUTPUT);
    pinMode(WINC1500_IRQ, INPUT);
    pinMode(WINC1500_RESET, OUTPUT);

    pinMode(GPS_POWER, OUTPUT);

    pinMode(LORA_ENABLE, OUTPUT);
    digitalWrite(LORA_ENABLE, LOW);

    for (auto pin : radio_spi_cs_pins) {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, HIGH);
    }

    for (auto pin : core_spi_cs_pins) {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, HIGH);
    }

    disable_everything();

    return true;
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
    return { "spi-flash", &SPI };
}

SpiWrapper Board::spi_sd() {
    return { "spi-sd", &SPI2 };
}

SpiWrapper Board::spi_radio() {
    return { "spi-radio", &SPI1 };
}

SpiWrapper Board::spi_module() {
    return { "spi-mod", nullptr };
}

TwoWireWrapper Board::i2c_core() {
    return { "i2c-core", &Wire };
}

TwoWireWrapper Board::i2c_radio() {
    return { "i2c-radio", &Wire1 };
}

TwoWireWrapper Board::i2c_module() {
    return { "i2c-mod", &Wire2 };
}

SerialWrapper Board::gps_serial() {
    return { "uart-gps", &Serial1 };
}

SpiWrapper::SpiWrapper(const char *name, void *ptr) : name_(name), ptr_(ptr) {
    logverbose("acquire %s", name_);
}

SpiWrapper::~SpiWrapper() {
    logverbose("release %s", name_);
}

void SpiWrapper::begin() {
    if (ptr_ == nullptr) return;

    reinterpret_cast<SPIClass*>(ptr_)->begin();
}

bool SpiWrapper::simple_command(uint8_t command) {
    return transfer_command(command, nullptr, nullptr, 0);
}

bool SpiWrapper::complex_command(uint8_t *command, uint32_t command_length) {
    return transfer(command, command_length, nullptr, nullptr, 0);
}

bool SpiWrapper::read_command(uint8_t command, uint8_t *data, uint32_t data_length) {
    return transfer_command(command, nullptr, data, data_length);
}

bool SpiWrapper::write_command(uint8_t command, uint8_t *data, uint32_t data_length) {
    return transfer_command(command, data, nullptr, data_length);
}

bool SpiWrapper::transfer_command(uint8_t command, const uint8_t *data_w, uint8_t *data_r, uint32_t data_length) {
    return transfer(&command, 1, data_w, data_r, data_length);
}


bool SpiWrapper::transfer(uint8_t *command, uint32_t command_length, const uint8_t *data_w, uint8_t *data_r, uint32_t data_length) {
    SPISettings spi_settings{ 50000000, MSBFIRST, SPI_MODE0 };
    auto bus = reinterpret_cast<SPIClass*>(ptr_);
    // enable();
    bus->beginTransaction(spi_settings);
    for (uint32_t i = 0; i < command_length; ++i) {
        bus->transfer(command[i]);
    }
    if (data_r != nullptr && data_w != nullptr) {
        for (uint32_t i = 0; i < data_length; ++i) {
            data_r[i] = bus->transfer(data_w[i]);
        }
    }
    else if (data_r != nullptr) {
        for (uint32_t i = 0; i < data_length; ++i) {
            data_r[i] = bus->transfer(0xff);
        }
    }
    else if (data_w != nullptr) {
        for (uint32_t i = 0; i < data_length; ++i) {
            bus->transfer(data_w[i]);
        }
    }
    bus->endTransaction();
    // disable();
    return true;
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

TwoWireWrapper::TwoWireWrapper(const char *name, void *ptr) : name_(name), ptr_(ptr) {
    logverbose("acquire %s", name_);
}

TwoWireWrapper::~TwoWireWrapper() {
    logverbose("release %s", name_);
}

void TwoWireWrapper::begin() {
    if (ptr_ == nullptr) return;

    reinterpret_cast<TwoWire*>(ptr_)->begin();
}

int32_t TwoWireWrapper::read(uint8_t address, void *data, int32_t size) {
    auto bus = reinterpret_cast<TwoWire*>(ptr_);
    bus->requestFrom(address, size);
    auto ptr = (uint8_t *)data;
    for (auto i = 0; i < size; ++i) {
        *ptr++ = bus->read();
    }
    return bus->endTransmission();
}

int32_t TwoWireWrapper::write(uint8_t address, const void *data, int32_t size) {
    auto bus = reinterpret_cast<TwoWire*>(ptr_);
    bus->beginTransmission(address);
    auto ptr = (uint8_t *)data;
    for (auto i = 0; i < size; ++i) {
        bus->write((uint8_t)*ptr++);
    }
    return bus->endTransmission();
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

SerialWrapper::SerialWrapper(const char *name, void *ptr) : name_(name), ptr_(ptr) {
    logverbose("acquire %s", name_);
}

SerialWrapper::~SerialWrapper() {
    logverbose("release %s", name_);
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

}

#endif
