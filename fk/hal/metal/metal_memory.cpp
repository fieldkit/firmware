#include "hal/metal/metal.h"
#include "board.h"

#if defined(ARDUINO)

namespace fk {

#define loginfo(f, ...)  loginfof("memory", f, ##__VA_ARGS__)

#define logerror(f, ...) logerrorf("memory", f, ##__VA_ARGS__)

constexpr uint8_t CMD_RESET = 0xff;
constexpr uint8_t CMD_READ_JEDEC_ID = 0x9f;
constexpr uint8_t CMD_ENABLE_WRITE = 0x06;
constexpr uint8_t CMD_DISABLE_WRITE = 0x04;
constexpr uint8_t CMD_GET_FEATURE = 0x0f;
constexpr uint8_t CMD_SET_FEATURE = 0x1f;
constexpr uint8_t CMD_READ_CELL_ARRAY = 0x13;
constexpr uint8_t CMD_READ_BUFFER = 0x03;
constexpr uint8_t CMD_ERASE_BLOCK = 0xd8;
constexpr uint8_t CMD_PROGRAM_LOAD = 0x02;
constexpr uint8_t CMD_PROGRAM_EXECUTE = 0x10;

constexpr uint8_t FLASH_JEDEC_MANUFACTURE = 0x98;
constexpr uint8_t FLASH_JEDEC_DEVICE = 0xcb;

constexpr uint8_t CMD_REGISTER_1 = 0xa0;
constexpr uint8_t CMD_REGISTER_2 = 0xb0;
constexpr uint8_t CMD_REGISTER_3 = 0xc0;

constexpr uint8_t STATUS_FLAG_BUSY = 0x1;
constexpr uint8_t STATUS_FLAG_WRITE_ENABLED = 0x1 << 1;
constexpr uint8_t STATUS_FLAG_ERASE_FAIL = 0x1 << 2;
constexpr uint8_t STATUS_FLAG_PROGRAM_FAIL = 0x1 << 3;

static SPISettings SpiSettings{ 50000000, MSBFIRST, SPI_MODE0 };

class FlashSpi {
private:
    uint8_t cs_;

public:
    FlashSpi(uint8_t cs) : cs_(cs) {
    }

public:
    typedef struct geometry_t {
        uint32_t page_size;
        uint32_t block_size;
        uint32_t nblocks;
    } geometry_t;

public:
    geometry_t get_geometry() const {
        return { 2112, (2112 * 64), 2048 };
    }

    bool begin() {
        SPI.begin();

        enable();
        simple_command(CMD_RESET);
        disable();

        // First byte is a dummy byte (p31)
        // TODO: Add SpiFlashTimeoutMs
        uint8_t jedec_id_response[3] = { 0xff, 0xff, 0xff };
        while (jedec_id_response[1] == 0xff) {
            read_command(CMD_READ_JEDEC_ID, jedec_id_response, 3);
        }
        FK_ASSERT(jedec_id_response[1] == FLASH_JEDEC_MANUFACTURE);
        FK_ASSERT(jedec_id_response[2] == FLASH_JEDEC_DEVICE);

        /* Unlock all blocks. */
        set_feature(CMD_REGISTER_1, 0xB8);
        set_feature(CMD_REGISTER_1, 0x00 | (0x1 << 7));

        /* Disable high speed read mode. */
        // set_feature(CMD_REGISTER_2, 0x14);

        if (false) {
            uint8_t features[] = { 0xA0, 0xB0, 0xC0 };
            for (auto a : features) {
                uint8_t value = 0x00;
                get_feature(a, &value);
                loginfo("feature-reg 0x%x = 0x%x", a, value);
            }
        }

        if (!is_ready()) {
            return false;
        }

        return true;
    }

    bool read(uint32_t address, uint8_t *data, uint32_t length) {
        uint8_t read_cell_command[] = { CMD_READ_CELL_ARRAY, 0x00, 0x00, 0x00 }; // 7dummy/17 (Row)
        uint8_t read_buffer_command[] = { CMD_READ_BUFFER, 0x00, 0x00, 0x00 };   // 4dummy/12/8dummy // (Col)

        row_address_to_bytes(address, read_cell_command + 1);
        column_address_to_bytes(address, read_buffer_command + 1);

        if (!is_ready()) {
            return false;
        }

        /* Load page into buffer. */
        if (!complex_command(read_cell_command, sizeof(read_cell_command))) {
            return false;
        }

        /* Wait for buffer to fill with data from cell array. */
        if (!is_ready()) {
            return false;
        }

        /* Read the buffer in. */
        if (!transfer(read_buffer_command, sizeof(read_buffer_command), nullptr, data, length)) {
            return false;
        }

        return true;
    }

    bool write(uint32_t address, uint8_t *data, uint32_t length) {
        uint8_t program_load_command[] = { CMD_PROGRAM_LOAD, 0x00, 0x00 }; // 4dummy/12
        uint8_t program_execute_command[] = { CMD_PROGRAM_EXECUTE, 0x00, 0x00, 0x00 }; // 7dummy/17

        column_address_to_bytes(address, program_load_command + 1);
        row_address_to_bytes(address, program_execute_command + 1);

        if (!is_ready()) {
            return false;
        }

        if (!enable_writes()) {
            return false;
        }

        if (!transfer(program_load_command, sizeof(program_load_command), data, nullptr, length)) {
            return false;
        }

        /* May be unnecessary. */
        if (!is_ready()) {
            return false;
        }

        if (!complex_command(program_execute_command, sizeof(program_execute_command))) {
            return false;
        }

        if (!is_ready()) {
            return false;
        }

        return true;
    }

    bool erase_block(uint32_t address) {
        uint8_t command[] = { CMD_ERASE_BLOCK, 0x00, 0x00, 0x00 }; // 7dummy/17 (Row)
        row_address_to_bytes(address, command + 1);

        if (!is_ready()) {
            return false;
        }

        if (!enable_writes()) {
            return false;
        }

        return transfer(command, sizeof(command), nullptr, nullptr, 0);
    }

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

    void row_address_to_bytes(uint32_t address, uint8_t *bytes) {
        bytes[2] = (address >> 16) & 0xff;
        bytes[1] = (address >> 8) & 0xff;
        bytes[0] = (address & 0xff);
    }

    void column_address_to_bytes(uint32_t address, uint8_t *bytes) {
        bytes[1] = (address >> 8) & 0xff;
        bytes[0] = (address & 0xff);
    }

    bool enable_writes() {
        enable();
        auto ok = simple_command(CMD_ENABLE_WRITE);
        disable();
        return ok;
    }

    bool disable_writes() {
        enable();
        auto ok = simple_command(CMD_DISABLE_WRITE);
        disable();
        return ok;
    }

    bool get_feature(uint8_t address, uint8_t *reg) {
        uint8_t command[] = { CMD_GET_FEATURE, address };
        enable();
        auto ok = transfer(command, sizeof(command), nullptr, reg, 1);
        disable();
        return ok;
    }

    bool set_feature(uint8_t address, uint8_t value) {
        uint8_t command[] = { CMD_SET_FEATURE, address, value };
        enable();
        auto ok = transfer(command, sizeof(command), nullptr, nullptr, 0);
        disable();
        return ok;
    }

    uint8_t read_status() {
        uint8_t status = 0x00;
        get_feature(CMD_REGISTER_3, &status);
        return status;
    }

    bool is_ready() {
        // TODO: Add SpiFlashTimeoutMs
        // TODO: We can send command and then read bytes in a loop.
        while (true) {
            auto status = read_status();
            if ((status & STATUS_FLAG_BUSY) != STATUS_FLAG_BUSY) {
                return true;
            }
            delay(1);
        }
        return false;
    }

    void enable() {
        // TODO: Mutex
        digitalWrite(cs_, LOW);
    }

    void disable() {
        // TODO: Mutex
        digitalWrite(cs_, HIGH);
    }

    bool simple_command(uint8_t command) {
        return transfer_command(command, nullptr, nullptr, 0);
    }

    bool complex_command(uint8_t *command, uint32_t command_length) {
        return transfer(command, command_length, nullptr, nullptr, 0);
    }

    bool read_command(uint8_t command, uint8_t *data, uint32_t data_length) {
        return transfer_command(command, nullptr, data, data_length);
    }

    bool write_command(uint8_t command, uint8_t *data, uint32_t data_length) {
        return transfer_command(command, data, nullptr, data_length);
    }

    bool transfer_command(uint8_t command, const uint8_t *data_w, uint8_t *data_r, uint32_t data_length) {
        return transfer(&command, 1, data_w, data_r, data_length);
    }

    bool transfer(uint8_t *command, uint32_t command_length, const uint8_t *data_w, uint8_t *data_r, uint32_t data_length) {
        enable();
        SPI.beginTransaction(SpiSettings);
        for (uint32_t i = 0; i < command_length; ++i) {
            SPI.transfer(command[i]);
        }
        if (data_r != nullptr && data_w != nullptr) {
            for (uint32_t i = 0; i < data_length; ++i) {
                data_r[i] = SPI.transfer(data_w[i]);
            }
        }
        else if (data_r != nullptr) {
            for (uint32_t i = 0; i < data_length; ++i) {
                data_r[i] = SPI.transfer(0xff);
            }
        }
        else if (data_w != nullptr) {
            for (uint32_t i = 0; i < data_length; ++i) {
                SPI.transfer(data_w[i]);
            }
        }
        SPI.endTransaction();
        disable();
        return true;
    }

};

MetalDataMemory::MetalDataMemory() {
}

static void dump_memory(uint8_t *ptr, size_t size) {
    fkb_external_printf("\n");
    for (size_t i = 0; i < size; ++i) {
        fkb_external_printf("0x%02x ", ptr[i]);
        if (((i + 1) % 32) == 0) {
            fkb_external_printf("\n");
        }
    }
    fkb_external_printf("\n");
}

bool MetalDataMemory::begin() {
    FlashSpi spi{ SPI_FLASH_CS_BANK_2 };

    FK_ASSERT(spi.begin());

    // Read a block of memory.
    uint32_t address = 0x0;
    uint8_t memory[2112] = { 0x00 };

    FK_ASSERT(spi.read(address, memory, sizeof(memory)));
    dump_memory(memory, sizeof(memory));

    FK_ASSERT(spi.erase_block(address));

    FK_ASSERT(spi.read(address, memory, sizeof(memory)));
    dump_memory(memory, sizeof(memory));

    for (size_t i = 0; i < sizeof(memory); ++i) {
        memory[i] = i & 0xff;
    }
    FK_ASSERT(spi.write(address, memory, sizeof(memory)));

    FK_ASSERT(spi.read(address, memory, sizeof(memory)));
    dump_memory(memory, sizeof(memory));

    return true;
}

}

#endif
