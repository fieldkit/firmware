#pragma once

#include "common.h"

namespace fk {

constexpr uint8_t WINC1500_CS = 95u;         // PC25
constexpr uint8_t WINC1500_POWER = 56u;      // PB07
constexpr uint8_t WINC1500_IRQ = 97u;        // PC27
constexpr uint8_t WINC1500_RESET = 96u;      // PC26

constexpr uint8_t GPS_POWER = 55u;

constexpr uint8_t QSPI_FLASH_CS = 90u;       // PB11 (PIN_QSPI_CS)

constexpr uint8_t PIN_SD_CS = 8u;            // PB18

constexpr uint8_t SPI_FLASH_CS_BANK_1 = 44u; // PC11
constexpr uint8_t SPI_FLASH_CS_BANK_2 = 41u; // PC12
constexpr uint8_t SPI_FLASH_CS_BANK_3 = 40u; // PC13
constexpr uint8_t SPI_FLASH_CS_BANK_4 = 43u; // PC14

constexpr uint8_t BUTTON_RIGHT = 32u;        // PA21
constexpr uint8_t BUTTON_MIDDLE = 31u;       // PA22
constexpr uint8_t BUTTON_LEFT = 30u;         // PA23
constexpr uint8_t BUTTON_EXTERNAL = 74u;     // PB04

constexpr uint8_t LORA_POWER = 76u;          // PC30

constexpr uint8_t MODULE_EEPROM_LOCK = 13u;  // PB01
constexpr uint8_t MODULE_SWAP = 69u;         // PB03

class SpiWrapper {
private:
    const char *name_;
    void *ptr_;

public:
    SpiWrapper(const char *name, void *ptr);
    ~SpiWrapper();

public:
    void begin();
    void end();

public:
    bool simple_command(uint8_t command);
    bool complex_command(uint8_t *command, uint32_t command_length);
    bool read_command(uint8_t command, uint8_t *data, uint32_t data_length);
    bool write_command(uint8_t command, uint8_t *data, uint32_t data_length);
    bool transfer_command(uint8_t command, const uint8_t *data_w, uint8_t *data_r, uint32_t data_length);
    bool transfer(uint8_t *command, uint32_t command_length, const uint8_t *data_w, uint8_t *data_r, uint32_t data_length);

};

union TwoWire16 {
    uint8_t bytes[2];
    uint16_t u16;
};

union TwoWire32 {
    uint8_t bytes[4];
    uint32_t u32;
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
    int32_t write_u8(uint8_t address, uint8_t value);
    int32_t write_register_u16(uint8_t address, uint8_t reg, uint16_t value);
    int32_t write_register_u32(uint8_t address, uint8_t reg, uint32_t value);
    int32_t read_register_u16(uint8_t address, uint8_t reg, uint16_t &value);

public:
    int32_t recover();

};

class AcquireTwoWireBus {
public:
    virtual TwoWireWrapper acquire() = 0;

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

class EepromLock {
private:
    uint32_t locked_;

public:
    explicit EepromLock();
    explicit EepromLock(uint32_t locked);
    EepromLock(EepromLock const &o);
    EepromLock(EepromLock &&o);
    virtual ~EepromLock();

};

class Board {
public:
    bool initialize();
    void disable_everything();
    void enable_everything();
    void disable_gps();
    void enable_gps();
    void disable_wifi();
    void enable_wifi();
    void disable_lora();
    void enable_lora();
    EepromLock lock_eeprom();
    void release_eeprom();
    void signal_eeprom(uint8_t times);

public:
    SpiWrapper spi_flash();
    SpiWrapper spi_sd();
    SpiWrapper spi_radio();
    SpiWrapper spi_module();
    TwoWireWrapper i2c_core();
    TwoWireWrapper i2c_radio();
    TwoWireWrapper i2c_module();
    SerialWrapper gps_serial();

public:
    AcquireTwoWireBus *acquire_i2c_radio();
    AcquireTwoWireBus *acquire_i2c_module();

};

Board *get_board();

}
