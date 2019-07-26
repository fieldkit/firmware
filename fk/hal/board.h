#pragma once

#include "common.h"

namespace fk {

constexpr uint8_t WINC1500_CS = 95u;         // PC25
constexpr uint8_t WINC1500_POWER = 56u;      // PB7
constexpr uint8_t WINC1500_IRQ = 97u;        // PC27
constexpr uint8_t WINC1500_RESET = 96u;      // PC26

constexpr uint8_t GPS_POWER = 55u;

constexpr uint8_t QSPI_FLASH_CS = 90u;       // PB11

constexpr uint8_t PIN_SD_CS = 8u;            // PB18

constexpr uint8_t SPI_FLASH_CS_BANK_1 = 44u; // PC11
constexpr uint8_t SPI_FLASH_CS_BANK_2 = 41u; // PC12
constexpr uint8_t SPI_FLASH_CS_BANK_3 = 40u; // PC13
constexpr uint8_t SPI_FLASH_CS_BANK_4 = 43u; // PC14

constexpr uint8_t BUTTON_RIGHT = 32u;        // PA21
constexpr uint8_t BUTTON_MIDDLE = 31u;       // PA22
constexpr uint8_t BUTTON_LEFT = 30u;         // PA23

class SpiWrapper {
private:
    const char *name_;
    void *ptr_;

public:
    void begin();
    void end();

public:
    SpiWrapper(const char *name, void *ptr);
    ~SpiWrapper();

};

class TwoWireWrapper {
private:
    const char *name_;
    void *ptr_;

public:
    TwoWireWrapper(const char *name, void *ptr);
    ~TwoWireWrapper();

public:
    void begin();
    void end();

public:
    int32_t read(uint8_t address, void *data, int32_t size);
    int32_t write(uint8_t address, const void *data, int32_t size);
    int32_t read_register_u8(uint8_t address, uint8_t reg, uint8_t &value);
    int32_t read_register_buffer(uint8_t address, uint8_t reg, uint8_t *buffer, int32_t size);
    int32_t write_register_u8(uint8_t address, uint8_t reg, uint8_t value);

};

class SerialWrapper {
private:
    const char *name_;
    void *ptr_;

public:
    SerialWrapper(const char *name, void *ptr);
    ~SerialWrapper();

public:
    bool begin(uint32_t baud);
    bool end();
    int32_t available();
    int8_t read();

};

class Board {
public:
    void initialize();
    void disable_everything();
    void enable_everything();
    void disable_gps();
    void enable_gps();
    void disable_wifi();
    void enable_wifi();

public:
    SpiWrapper spi_flash();
    SpiWrapper spi_sd();
    SpiWrapper spi_radio();
    SpiWrapper spi_module();
    TwoWireWrapper i2c_core();
    TwoWireWrapper i2c_radio();
    TwoWireWrapper i2c_module();
    SerialWrapper gps_serial();

};

Board *get_board();

}
