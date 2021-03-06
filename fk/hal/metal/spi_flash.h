#pragma once

#include "common.h"
#include "hal/memory.h"

namespace fk {

enum class SpiFlashError {
    None,
    Program,
    Erase
};

class SpiFlash {
private:
    constexpr static uint32_t SpiFlashReadyMs = 10;
    constexpr static uint32_t SpiFlashTimeoutMs = 500;
    constexpr static uint32_t IdSize = 32;

public:
    constexpr static uint32_t PageSize = 2048;
    constexpr static uint32_t BlockSize = 2048 * 64;
    constexpr static uint32_t NumberOfBlocks = 2048;

private:
    Availability status_{ Availability::Unknown };
    uint8_t cs_{ 0 };
    uint8_t id_[IdSize]{ };
    SpiFlashError error_{ SpiFlashError::None };

public:
    explicit SpiFlash(uint8_t cs);

public:
    FlashGeometry geometry() const;

    bool begin();

    int32_t read(uint32_t address, uint8_t *data, size_t length);

    int32_t write(uint32_t address, const uint8_t *data, size_t length);

    int32_t erase_block(uint32_t address);

    const uint8_t *id() const {
        return id_;
    }

private:
    // cppcheck-suppress unusedPrivateFunction
    const SpiFlashError error() const {
        return error_;
    }

    /*
      Row Address: 17 bits
      Block Address (2048 blocks/device): 11 bits
      Page Address (64 pages/block): 6 bits
      Column Address: 12 bits
      Column Address (2112 or 2176 bytes/page): 12 bits

      MSB           LSB
      Block Page Column
      ROW    ROW
    */

    int32_t write_internal(uint32_t address, const uint8_t *data, size_t length);

    int32_t erase_block_internal(uint32_t address);

    void row_address_to_bytes(uint32_t address, uint8_t *bytes);

    void column_address_to_bytes(uint32_t address, uint8_t *bytes);

    bool enable_writes();

    // cppcheck-suppress unusedPrivateFunction
    bool disable_writes();

    bool get_feature(uint8_t address, uint8_t *reg);

    bool set_feature(uint8_t address, uint8_t value);

    // cppcheck-suppress unusedPrivateFunction
    bool read_parameters_page();

    // cppcheck-suppress unusedPrivateFunction
    bool read_unique_id();

    uint8_t read_status();

    bool is_ready(bool ecc_check = false);

    void enable();

    void disable();

    // cppcheck-suppress unusedPrivateFunction
    void ecc_check();

    void read_ecc_information();

    bool simple_command(uint8_t command);

    bool complex_command(uint8_t *command, uint32_t command_length);

    bool read_command(uint8_t command, uint8_t *data, uint32_t data_length);

    // cppcheck-suppress unusedPrivateFunction
    bool write_command(uint8_t command, uint8_t *data, uint32_t data_length);

    bool transfer_command(uint8_t command, const uint8_t *data_w, uint8_t *data_r, uint32_t data_length);

    bool transfer(uint8_t *command, uint32_t command_length, const uint8_t *data_w, uint8_t *data_r, uint32_t data_length);

    bool reset();

    void dump_feature_registers();

};

}
