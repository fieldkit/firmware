#include "hal/board.h"
#include "platform.h"
#include "modules/shared/modules.h"

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
    PIN_WIRE2_SDA, PIN_WIRE2_SCL,
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

    pinMode(LORA_POWER, OUTPUT);
    digitalWrite(LORA_POWER, LOW);

    for (auto pin : radio_spi_cs_pins) {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, HIGH);
    }

    for (auto pin : core_spi_cs_pins) {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, HIGH);
    }

    pinMode(MODULE_EEPROM_LOCK, OUTPUT);
    digitalWrite(MODULE_EEPROM_LOCK, LOW);

    pinMode(MODULE_SOLO_ENABLE, OUTPUT);
    digitalWrite(MODULE_SOLO_ENABLE, HIGH);

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
    disable_lora();
}

void Board::enable_everything() {
    enable_gps();
    enable_wifi();
    enable_lora();
}

void Board::disable_gps() {
    digitalWrite(GPS_POWER, LOW);
}

void Board::enable_gps() {
    digitalWrite(GPS_POWER, HIGH);
}

void Board::disable_lora() {
    digitalWrite(LORA_POWER, LOW);
}

void Board::enable_lora() {
    digitalWrite(LORA_POWER, HIGH);
}

void Board::disable_wifi() {
    digitalWrite(WINC1500_POWER, LOW);
    SPI1.end();
}

void Board::enable_wifi() {
    digitalWrite(WINC1500_POWER, HIGH);
    SPI1.begin();
}

EepromLock Board::lock_eeprom() {
    digitalWrite(MODULE_EEPROM_LOCK, HIGH);

    // See the documentation of this define for more information.
    fk_delay(FK_MODULES_EEPROM_WRITE_TIME);

    return EepromLock{ fk_uptime() };
}

void Board::release_eeprom() {
    digitalWrite(MODULE_EEPROM_LOCK, LOW);
}

void Board::signal_eeprom(uint8_t times) {
    for (auto i = 0; i < times; ++i) {
        digitalWrite(MODULE_EEPROM_LOCK, HIGH);
        fk_delay(5);
        digitalWrite(MODULE_EEPROM_LOCK, LOW);
        fk_delay(5);
    }
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

    __disable_irq();

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

    __enable_irq();

    return true;
}

void SpiWrapper::end() {
    if (ptr_ == nullptr) return;

    // When we call this during startup, deep sleeping the MCU fails,
    // some sort of rogue IRQ?
    reinterpret_cast<SPIClass*>(ptr_)->end();

    // NOTE None of the following seems to affect the problem with
    // deep sleep. So right now we're calling disableSPI instead of
    // resetSPI inside the Arduino library file SPI.cpp
    if (ptr_ == &SPI) {
        pinMode(PIN_SPI_MISO, INPUT_PULLUP);
        pinMode(PIN_SPI_MOSI, INPUT_PULLUP);
        pinMode(PIN_SPI_SCK, INPUT_PULLUP);

        NVIC_DisableIRQ(SERCOM4_0_IRQn);
        NVIC_DisableIRQ(SERCOM4_1_IRQn);
        NVIC_DisableIRQ(SERCOM4_2_IRQn);
        NVIC_DisableIRQ(SERCOM4_3_IRQn);

        NVIC_ClearPendingIRQ(SERCOM4_0_IRQn);
        NVIC_ClearPendingIRQ(SERCOM4_1_IRQn);
        NVIC_ClearPendingIRQ(SERCOM4_2_IRQn);
        NVIC_ClearPendingIRQ(SERCOM4_3_IRQn);

        // PERIPH_SPI.disableSPI();
    }
    else if (ptr_ == &SPI1) {
        pinMode(PIN_SPI1_MISO, INPUT_PULLUP);
        pinMode(PIN_SPI1_MOSI, INPUT_PULLUP);
        pinMode(PIN_SPI1_SCK, INPUT_PULLUP);

        NVIC_DisableIRQ(SERCOM2_0_IRQn);
        NVIC_DisableIRQ(SERCOM2_1_IRQn);
        NVIC_DisableIRQ(SERCOM2_2_IRQn);
        NVIC_DisableIRQ(SERCOM2_3_IRQn);

        NVIC_ClearPendingIRQ(SERCOM2_0_IRQn);
        NVIC_ClearPendingIRQ(SERCOM2_1_IRQn);
        NVIC_ClearPendingIRQ(SERCOM2_2_IRQn);
        NVIC_ClearPendingIRQ(SERCOM2_3_IRQn);

        // PERIPH_SPI1.disableSPI();
    }
    else if (ptr_ == &SPI2) {
        pinMode(PIN_SPI2_MISO, INPUT_PULLUP);
        pinMode(PIN_SPI2_MOSI, INPUT_PULLUP);
        pinMode(PIN_SPI2_SCK, INPUT_PULLUP);

        NVIC_DisableIRQ(SERCOM5_0_IRQn);
        NVIC_DisableIRQ(SERCOM5_1_IRQn);
        NVIC_DisableIRQ(SERCOM5_2_IRQn);
        NVIC_DisableIRQ(SERCOM5_3_IRQn);

        NVIC_ClearPendingIRQ(SERCOM5_0_IRQn);
        NVIC_ClearPendingIRQ(SERCOM5_1_IRQn);
        NVIC_ClearPendingIRQ(SERCOM5_2_IRQn);
        NVIC_ClearPendingIRQ(SERCOM5_3_IRQn);

        // PERIPH_SPI2.disableSPI();
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

    __disable_irq();

    reinterpret_cast<TwoWire*>(ptr_)->begin();

    __enable_irq();
}

int32_t TwoWireWrapper::read(uint8_t address, void *data, int32_t size) {
    __disable_irq();

    auto bus = reinterpret_cast<TwoWire*>(ptr_);
    bus->requestFrom(address, size);
    auto ptr = (uint8_t *)data;
    for (auto i = 0; i < size; ++i) {
        *ptr++ = bus->read();
    }
    auto rv = bus->endTransmission();

    __enable_irq();

    return rv;
}

int32_t TwoWireWrapper::write(uint8_t address, const void *data, int32_t size) {
    __disable_irq();

    auto bus = reinterpret_cast<TwoWire*>(ptr_);
    bus->beginTransmission(address);
    auto ptr = (uint8_t *)data;
    for (auto i = 0; i < size; ++i) {
        bus->write((uint8_t)*ptr++);
    }
    auto rv = bus->endTransmission();

    __enable_irq();

    return rv;
}

static void i2c_end(TwoWire *ptr) {
    ptr->end();

    if (ptr == &Wire) {
        pinMode(PIN_WIRE_SDA, INPUT);
        pinMode(PIN_WIRE_SCL, INPUT);
    }
    else if (ptr == &Wire1) {
        pinMode(PIN_WIRE1_SDA, INPUT);
        pinMode(PIN_WIRE1_SCL, INPUT);
    }
    else if (ptr == &Wire2) {
        pinMode(PIN_WIRE2_SDA, INPUT);
        pinMode(PIN_WIRE2_SCL, INPUT);
    }
}

void TwoWireWrapper::end() {
    if (ptr_ == nullptr) return;

    __disable_irq();

    i2c_end(reinterpret_cast<TwoWire*>(ptr_));

    __enable_irq();
}

static bool i2c_recover(uint8_t scl, uint8_t sda) {
    auto scl_low = digitalRead(scl) == LOW;
    if (scl_low) {
        logerror("i2c scl low, this may fail.");
    }

    auto tries = 10;
    auto sda_low = digitalRead(sda) == LOW;
    if (!sda_low) {
        loginfo("i2c recover sda is good, why are we here?");
        return true;
    }

    while (sda_low && tries > 0) {
        pinMode(sda, OUTPUT);
        delayMicroseconds(10);
        digitalWrite(sda, HIGH);
        delayMicroseconds(10);
        sda_low = digitalRead(sda) == LOW;
        if (!sda_low) {
            loginfo("sda is back");
            return true;
        }

        pinMode(sda, INPUT);
        delayMicroseconds(10);

        pinMode(scl, OUTPUT);
        digitalWrite(scl, HIGH);
        delayMicroseconds(10);
        digitalWrite(scl, LOW);
        delayMicroseconds(10);
        digitalWrite(scl, HIGH);
        delayMicroseconds(10);

        tries--;
    }

    loginfo("i2c recover failed");

    return false;
}

int32_t TwoWireWrapper::recover() {
    __disable_irq();

    logwarn("trying to recover i2c bus");

    auto tw = reinterpret_cast<TwoWire*>(ptr_);

    i2c_end(tw);

    if (ptr_ == &Wire) {
        i2c_recover(PIN_WIRE_SCL, PIN_WIRE_SDA);
    }
    else if (ptr_ == &Wire1) {
        i2c_recover(PIN_WIRE1_SCL, PIN_WIRE1_SDA);
    }
    else if (ptr_ == &Wire2) {
        i2c_recover(PIN_WIRE2_SCL, PIN_WIRE2_SDA);
    }
    else {
        FK_ASSERT(0);
    }

    tw->begin();

    __enable_irq();

    return 0;
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
    __disable_irq();

    auto rv = reinterpret_cast<Uart*>(ptr_)->available();

    __enable_irq();

    return rv;
}

int8_t SerialWrapper::read() {
    __disable_irq();

    auto rv = reinterpret_cast<Uart*>(ptr_)->read();

    __enable_irq();

    return rv;
}

}

#endif
