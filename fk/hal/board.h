#pragma once

#include "common.h"
#include "hal/mutex.h"
#include "hal/pins.h"

namespace fk {

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

enum class TwoWireFlags : uint32_t {
    None = 0,
    Release = 1,
    HoldOnRW = 2,
};

class TwoWireWrapper {
private:
    Mutex *lock_{ nullptr };
    const char *name_;
    void *ptr_;

public:
    TwoWireWrapper(Mutex *lock, const char *name, void *ptr);
    virtual ~TwoWireWrapper();

public:
    void begin();
    void end();

public:
    int32_t read(uint8_t address, void *data, int32_t size, TwoWireFlags flags = TwoWireFlags::Release);
    int32_t write(uint8_t address, void const *data, int32_t size, TwoWireFlags flags = TwoWireFlags::Release);
    int32_t read_register_u8(uint8_t address, uint8_t reg, uint8_t &value, TwoWireFlags flags = TwoWireFlags::None);
    int32_t read_register_buffer(uint8_t address, uint8_t reg, uint8_t *buffer, int32_t size, TwoWireFlags flags = TwoWireFlags::None);
    int32_t write_register_u8(uint8_t address, uint8_t reg, uint8_t value, TwoWireFlags flags = TwoWireFlags::None);
    int32_t write_register_buffer(uint8_t address, uint8_t reg, void const *data, int32_t size, TwoWireFlags flags = TwoWireFlags::None);
    int32_t write_u8(uint8_t address, uint8_t value, TwoWireFlags flags = TwoWireFlags::None);
    int32_t write_register_u16(uint8_t address, uint8_t reg, uint16_t value, TwoWireFlags flags = TwoWireFlags::None);
    int32_t write_register_u32(uint8_t address, uint8_t reg, uint32_t value, TwoWireFlags flags = TwoWireFlags::None);
    int32_t read_register_u16(uint8_t address, uint8_t reg, uint16_t &value, TwoWireFlags flags = TwoWireFlags::None);

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

class Board {
public:
    bool initialize();
    void disable_everything();
    void enable_everything();
    void disable_gps();
    void enable_gps();

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
    AcquireTwoWireBus *acquire_i2c_core();
    AcquireTwoWireBus *acquire_i2c_radio();
    AcquireTwoWireBus *acquire_i2c_module();
};

Board *get_board();

} // namespace fk
