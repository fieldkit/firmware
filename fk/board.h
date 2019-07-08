#pragma once

#include "common.h"

namespace fk {

constexpr uint8_t WINC1500_CS = 54u;  /* This is moving */
// constexpr uint8_t WINC1500_ENABLE = 95u;
constexpr uint8_t WINC1500_IRQ = 97u;
constexpr uint8_t WINC1500_RESET = 96u;
constexpr uint8_t WINC1500_POWER = 56u;

constexpr uint8_t GPS_POWER = 55u;

constexpr uint8_t QSPI_FLASH_CS = 90u; // PB11

constexpr uint8_t PIN_SD_CS = 8u;  // PB18

constexpr uint8_t SPI_FLASH_CS_BANK_1 = 44u; // PC11
constexpr uint8_t SPI_FLASH_CS_BANK_2 = 41u; // PC12
constexpr uint8_t SPI_FLASH_CS_BANK_3 = 40u; // PC13
constexpr uint8_t SPI_FLASH_CS_BANK_4 = 43u; // PC14

constexpr uint8_t BUTTON_RIGHT = 32u;        // PA21
constexpr uint8_t BUTTON_MIDDLE = 31u;       // PA22
constexpr uint8_t BUTTON_LEFT = 30u;         // PA23

class SpiWrapper {
private:
    void *ptr_;

public:
    void begin();
    void end();

public:
    SpiWrapper(void *ptr) : ptr_(ptr) {
    }

};

class TwoWireWrapper {
private:
    void *ptr_;

public:
    void begin();
    void end();

public:
    TwoWireWrapper(void *ptr) : ptr_(ptr) {
    }

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
    SpiWrapper spi_radio();
    SpiWrapper spi_module();
    TwoWireWrapper i2c_core();
    TwoWireWrapper i2c_radio();
    TwoWireWrapper i2c_module();

};

extern Board board;

}
