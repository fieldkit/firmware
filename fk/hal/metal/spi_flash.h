#pragma once

#include "common.h"
#include "hal/memory.h"

namespace fk {

class SpiFlash {
private:
    constexpr static uint32_t PageSize = 2048;
    constexpr static uint32_t BlockSize = 2048 * 64;
    constexpr static uint32_t NumberOfBlocks = 2048;
    constexpr static uint32_t SpiFlashReadyMs = 10;
    constexpr static uint32_t SpiFlashTimeoutMs = 500;

    enum class Status {
        Unknown,
        Available,
        Unavailable
    };

private:
    Status status_{ Status::Unknown };
    uint8_t cs_;

public:
    SpiFlash(uint8_t cs);

public:
    flash_geometry_t geometry() const;

    bool begin();

    int32_t read(uint32_t address, uint8_t *data, size_t length);

    int32_t write(uint32_t address, const uint8_t *data, size_t length);

    int32_t erase_block(uint32_t address);

private:
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

    void row_address_to_bytes(uint32_t address, uint8_t *bytes);

    void column_address_to_bytes(uint32_t address, uint8_t *bytes);

    bool enable_writes();

    bool disable_writes();

    bool get_feature(uint8_t address, uint8_t *reg);

    bool set_feature(uint8_t address, uint8_t value);

    uint8_t read_status();

    bool is_ready();

    void enable();

    void disable();

    bool simple_command(uint8_t command);

    bool complex_command(uint8_t *command, uint32_t command_length);

    bool read_command(uint8_t command, uint8_t *data, uint32_t data_length);

    bool write_command(uint8_t command, uint8_t *data, uint32_t data_length);

    bool transfer_command(uint8_t command, const uint8_t *data_w, uint8_t *data_r, uint32_t data_length);

    bool transfer(uint8_t *command, uint32_t command_length, const uint8_t *data_w, uint8_t *data_r, uint32_t data_length);

};

}
