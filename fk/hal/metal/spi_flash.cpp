#include "spi_flash.h"
#include "platform.h"
#include "utilities.h"
#include "hal/metal/metal_ipc.h"

#if defined(ARDUINO)

#include <SPI.h>

namespace fk {

FK_DECLARE_LOGGER("spiflash");

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
constexpr uint8_t CMD_PROGRAM_LOAD_RANDOM = 0x84;
constexpr uint8_t CMD_PROGRAM_EXECUTE = 0x10;

constexpr uint8_t FLASH_JEDEC_MANUFACTURE = 0x98;
constexpr uint8_t FLASH_JEDEC_DEVICE = 0xcb;
constexpr uint8_t FLASH_JEDEC_MANUFACTURE_KIOXIA = 0x98;
constexpr uint8_t FLASH_JEDEC_DEVICE_KIOXIA = 0xe4;

constexpr uint8_t CMD_REGISTER_1 = 0xa0;
constexpr uint8_t CMD_REGISTER_2 = 0xb0;
constexpr uint8_t CMD_REGISTER_3 = 0xc0;
constexpr uint8_t CMD_ECC_STATUS_0 = 0x40;
constexpr uint8_t CMD_ECC_STATUS_1 = 0x50;

constexpr uint8_t STATUS_FLAG_BUSY = 0x1;
constexpr uint8_t STATUS_FLAG_WRITE_ENABLED = 0x1 << 1;
constexpr uint8_t STATUS_FLAG_ERASE_FAIL = 0x1 << 2;
constexpr uint8_t STATUS_FLAG_PROGRAM_FAIL = 0x1 << 3;
constexpr uint8_t STATUS_FLAG_ECC_STATUS_MASK = (0x1 << 4) | (0x1 << 5);
constexpr uint8_t STATUS_FLAG_ECC_STATUS_Pos = (4);

static SPISettings SpiSettings{ 50000000, MSBFIRST, SPI_MODE0 };

SpiFlash::SpiFlash(uint8_t cs) : cs_(cs) {
}

FlashGeometry SpiFlash::geometry() const {
    if (status_ != Availability::Available) {
        return { 0, 0, 0, 0 };
    }
    return { PageSize, BlockSize, NumberOfBlocks, NumberOfBlocks * BlockSize };
}

static bool check_jedec(uint8_t *jedec) {
    if (jedec[1] == FLASH_JEDEC_MANUFACTURE || jedec[2] == FLASH_JEDEC_DEVICE) {
        return true;
    }
    if (jedec[1] == FLASH_JEDEC_MANUFACTURE_KIOXIA || jedec[2] == FLASH_JEDEC_DEVICE_KIOXIA) {
        return true;
    }
    logwarn("unexpected jedec: 0x%02x%02x%02x%02x", jedec[0], jedec[1], jedec[2], jedec[3]);
    return false;
}

bool SpiFlash::begin() {
    if (status_ != Availability::Unknown) {
        return status_ == Availability::Available;
    }

    // auto lock = spi_flash_mutex.acquire(UINT32_MAX);
    // FK_ASSERT(lock);

    status_ = Availability::Unavailable;

    SPI.begin();

    if (!reset()) {
        logerror("error resetting chip");
        return false;
    }

    // First byte is a dummy byte (p31)
    auto started = fk_uptime();
    uint8_t jedec_id[4] = { 0xff, 0xff, 0xff, 0xff };
    while (jedec_id[1] == 0xff) {
        read_command(CMD_READ_JEDEC_ID, jedec_id, sizeof(jedec_id));
        if (fk_uptime() - started > SpiFlashReadyMs) {
            logerror("error reading jedec");
            return false;
        }
    }
    if (!check_jedec(jedec_id)) {
        return false;
    }

    dump_feature_registers();

    if (false) {
        if (!read_unique_id()) {
            return false;
        }

        if (!read_parameters_page()) {
            return false;
        }
    }

    if (false) {
        char id_string[sizeof(id_) * 2];
        bytes_to_hex_string(id_string, sizeof(id_string), id_, sizeof(id_));
        loginfo("%s", id_string);
    }

    status_ = Availability::Available;

    return true;
}

void SpiFlash::dump_feature_registers() {
    uint8_t features[] = { 0xA0, 0xB0, 0xC0 };
    for (auto a : features) {
        uint8_t value = 0x00;
        get_feature(a, &value);
        logverbose("feature-reg 0x%x = 0x%x", a, value);
    }
}

bool SpiFlash::reset() {
    enable();
    auto ok = simple_command(CMD_RESET);
    disable();

    if (!is_ready()) {
        logerror("read: !ready");
        return false;
    }

    /* Unlock all blocks. */
    set_feature(CMD_REGISTER_1, 0xB8);
    set_feature(CMD_REGISTER_1, 0x00 | (0x1 << 7));

    /* Disable ECC. */
    // set_feature(CMD_REGISTER_2, 0b00000110);

    return ok;
}

int32_t SpiFlash::read(uint32_t address, uint8_t *data, size_t length) {
    // FK_ASSERT_LE((address % PageSize) + length, PageSize);

    uint8_t read_cell_command[] = { CMD_READ_CELL_ARRAY, 0x00, 0x00, 0x00 }; // 7dummy/17 (Row)
    uint8_t read_buffer_command[] = { CMD_READ_BUFFER, 0x00, 0x00, 0x00 };   // 4dummy/12/8dummy // (Col)

    row_address_to_bytes(address, read_cell_command + 1);
    column_address_to_bytes(address, read_buffer_command + 1);

    // auto lock = spi_flash_mutex.acquire(UINT32_MAX);
    // FK_ASSERT(lock);

    if (!is_ready()) {
        logerror("read: !ready");
        return 0;
    }

    /* Disable high speed read mode. */
    // set_feature(CMD_REGISTER_2, 0x14);

    /* Load page into buffer. */
    if (!complex_command(read_cell_command, sizeof(read_cell_command))) {
        logerror("read: read cell failed");
        return 0;
    }

    /* Wait for buffer to fill with data from cell array. */
    if (!is_ready(false)) {
        logerror("read: read cell !ready");
        return 0;
    }

    /* Read the buffer in. */
    if (!transfer(read_buffer_command, sizeof(read_buffer_command), nullptr, data, length)) {
        logerror("read: read buffer failed");
        return 0;
    }

    return length;
}

int32_t SpiFlash::write(uint32_t address, const uint8_t *data, size_t length) {
    auto wrote = write_internal(address, data, length);
    if (wrote > 0) {
        return wrote;
    }

    if (error_ == SpiFlashError::Program) {
        loginfo("resetting");
        if (!reset()) {
            logerror("resetting failed");
        }
        return 0;
    }

    logerror("write failed, trying to recover...");

    status_ = Availability::Unknown;

    if (!begin()) {
        logerror("begin failed!");
        return 0;
    }

    return write_internal(address, data, length);
}

int32_t SpiFlash::write_internal(uint32_t address, const uint8_t *data, size_t length) {
    FK_ASSERT_LE((address % PageSize) + length, PageSize);

    uint8_t program_load_command[] = { CMD_PROGRAM_LOAD, 0x00, 0x00 }; // 4dummy/12
    uint8_t program_execute_command[] = { CMD_PROGRAM_EXECUTE, 0x00, 0x00, 0x00 }; // 7dummy/17

    row_address_to_bytes(address, program_execute_command + 1);
    column_address_to_bytes(address, program_load_command + 1);

    logverbose("[0x%08" PRIx32 "] write: length=%d (end = [0x%06" PRIx32 "])", address, length, address + length);

    // auto lock = spi_flash_mutex.acquire(UINT32_MAX);
    // FK_ASSERT(lock);

    if (!is_ready()) {
        logerror("flush: !ready");
        return 0;
    }

    if (!enable_writes()) {
        logerror("flush: enabling writes failed");
        return 0;
    }

    if (!transfer(program_load_command, sizeof(program_load_command), data, nullptr, length)) {
        logerror("flush: program load failed");
        return 0;
    }

    /* May be unnecessary. */
    if (!is_ready()) {
        logerror("flush: program load !ready");
        return 0;
    }

    if (!complex_command(program_execute_command, sizeof(program_execute_command))) {
        logerror("flush: program execute failed");
        return 0;
    }

    if (!is_ready()) {
        logerror("flush: program execute !ready");
        return 0;
    }

    return length;
}

int32_t SpiFlash::erase_block(uint32_t address) {
    auto rv = erase_block_internal(address);
    if (rv <= 0) {
        if (error_ == SpiFlashError::Erase) {
            loginfo("resetting");
            if (!reset()) {
                logerror("resetting failed");
            }
        }
    }
    return rv;
}

int32_t SpiFlash::erase_block_internal(uint32_t address) {
    uint8_t command[] = { CMD_ERASE_BLOCK, 0x00, 0x00, 0x00 }; // 7dummy/17 (Row)
    row_address_to_bytes(address, command + 1);

    // auto lock = spi_flash_mutex.acquire(UINT32_MAX);
    // FK_ASSERT(lock);

    logverbose("[0x%06" PRIx32 "] erase", address);

    if (!is_ready()) {
        logerror("erase: !ready");
        if (reset()) {
            logerror("erase: reset failed");
        }
        return 0;
    }

    if (!enable_writes()) {
        logerror("erase: enable writes failed");
        return 0;
    }

    if (!transfer(command, sizeof(command), nullptr, nullptr, 0)) {
        logerror("erase: erase failed [0x%06" PRIx32 "]", address);
        return 0;
    }

    if (!is_ready()) {
        logerror("erase: erase !ready");
        return 0;
    }

    return 1;
}

struct __attribute__ ((packed)) parameters_page_t {
    uint8_t signature[4]; // 4e 41 4e 44
    uint8_t reserved_1[31 - 4 + 1];
    uint8_t manufacturer[12];
    uint8_t model[20];
    uint8_t manufacturer_id;
    uint8_t reserved_2[79 - 65 + 1];
    uint32_t bytes_per_page;
    uint16_t spare_bytes_per_page;
    uint32_t bytes_per_partial_page;
    uint16_t spare_bytes_per_partial_page;
    uint32_t pages_per_block;
    uint32_t blocks_per_device;
    uint8_t number_of_devices;
    uint8_t reserved_3[1];
    uint8_t bits_per_cell;
    uint16_t max_bad_blocks_per_device;
    uint16_t block_endurance;
    uint8_t guaranteed_valid_at_beginning;
    uint8_t reserved_4[2];
    uint8_t number_of_programs_per_page;
    uint8_t reserved_5[1];
    uint8_t number_of_ecc_bits;
    uint8_t reserved_6[127 - 113 + 1];
    uint8_t io_pin_capacitance;
    uint8_t reserved_7[132 - 129 + 1];
    uint16_t max_program_page_time;
    uint16_t max_block_erase_time;
    uint16_t max_page_read_time;
    uint8_t reserved_8[253 - 139 + 1];
    uint16_t crc;
};

bool SpiFlash::read_parameters_page() {
    constexpr static uint8_t ENABLE_READ_PARAMETERS_PAGE = 0b1010110;
    constexpr static uint8_t DISABLE_READ_PARAMETERS_PAGE = 0b10110;

    static_assert(sizeof(parameters_page_t) == 256, "unexpected parameters page size");

    uint8_t read_cell_command[] = { CMD_READ_CELL_ARRAY, 0x00, 0x00, 0x00 }; // 7dummy/17 (Row)
    uint8_t read_buffer_command[] = { CMD_READ_BUFFER, 0x00, 0x00, 0x00 };   // 4dummy/12/8dummy (Col)

    row_address_to_bytes(PageSize, read_cell_command + 1);

    // 1. Set Feature (1Fh) with address B0h and set bit [6]. : To set the IDR_E bit in the feature table.
    // 2. Read Cell Array (13h) with address 01h. : To read the parameter page.
    // 3. Get Feature (0Fh) : To read the status (OIP bit) of the device.
    // 4. Read Buffer (03h or 0Bh) with address 00h.
    // 5. Set Feature (1Fh) with address B0h and clear bit [6]. : To clear the IDR_E bit in the feature table.

    set_feature(CMD_REGISTER_2, ENABLE_READ_PARAMETERS_PAGE);

    /* Load page into buffer. */
    if (!complex_command(read_cell_command, sizeof(read_cell_command))) {
        return 0;
    }

    /* Wait for buffer to fill with data from cell array. */
    if (!is_ready(false)) {
        return 0;
    }

    parameters_page_t page;

    /* Read the buffer in. */
    if (!transfer(read_buffer_command, sizeof(read_buffer_command), nullptr, (uint8_t *)&page, sizeof(page))) {
        return 0;
    }

    set_feature(CMD_REGISTER_2, DISABLE_READ_PARAMETERS_PAGE);

    if (true) {
        loginfo("bytes_per_page: %" PRIu32, page.bytes_per_page);
        loginfo("spare_bytes_per_page: %d", page.spare_bytes_per_page);
        loginfo("bytes_per_partial_page: %" PRIu32, page.bytes_per_partial_page);
        loginfo("spare_bytes_per_partial_page: %d", page.spare_bytes_per_partial_page);
        loginfo("pages_per_block: %" PRIu32, page.pages_per_block);
        loginfo("blocks_per_device: %" PRIu32, page.blocks_per_device);
        loginfo("number_of_programs_per_page: %d", page.number_of_programs_per_page);
    }

    return true;
}

bool SpiFlash::read_unique_id() {
    constexpr static uint8_t ENABLE_READ_ID_PAGE = 0b1010110;
    constexpr static uint8_t DISABLE_READ_ID_PAGE = 0b10110;

    uint8_t read_cell_command[] = { CMD_READ_CELL_ARRAY, 0x00, 0x00, 0x00 }; // 7dummy/17 (Row)
    uint8_t read_buffer_command[] = { CMD_READ_BUFFER, 0x00, 0x00, 0x00 };   // 4dummy/12/8dummy (Col)

    // 1. Set Feature (1Fh) with address B0h and set bit [6]. : To set the IDR_E bit in the feature table.
    // 2. Read Cell Array (13h) with address 00h. : To read the unique ID.
    // 3. Get Feature (0Fh) : To read the status (OIP bit) of the device.
    // 4. Read Buffer (03h or 0Bh) with address 00h. : To output the 16 copies of the Unique ID.
    // 5. Set Feature (1Fh) with address B0h and clear bit [6]

    set_feature(CMD_REGISTER_2, ENABLE_READ_ID_PAGE);

    /* Load page into buffer. */
    if (!complex_command(read_cell_command, sizeof(read_cell_command))) {
        return 0;
    }

    /* Wait for buffer to fill with data from cell array. */
    if (!is_ready(false)) {
        return 0;
    }

    /* Read the buffer in. */
    if (!transfer(read_buffer_command, sizeof(read_buffer_command), nullptr, id_, sizeof(id_))) {
        return 0;
    }

    set_feature(CMD_REGISTER_2, DISABLE_READ_ID_PAGE);

    if (false) {
        fk_dump_memory("id ", &id_[              0], sizeof(id_) / 2);
        fk_dump_memory("id ", &id_[sizeof(id_) / 2], sizeof(id_) / 2);
    }

    return true;
}

void SpiFlash::row_address_to_bytes(uint32_t address, uint8_t *bytes) {
    uint32_t row = (address / PageSize);
    bytes[0] = (row >> 16) & 0xff;
    bytes[1] = (row >> 8) & 0xff;
    bytes[2] = (row & 0xff);
}

void SpiFlash::column_address_to_bytes(uint32_t address, uint8_t *bytes) {
    uint32_t column = (address % PageSize);
    bytes[0] = (column >> 8) & 0xff;
    bytes[1] = (column & 0xff);
}

void SpiFlash::ecc_check() {
    auto status = read_status();

    auto ecc = (((status) & STATUS_FLAG_ECC_STATUS_MASK) >> STATUS_FLAG_ECC_STATUS_Pos);
    if (ecc > 0) {
        logwarn("ecc status flags: 0x%x (0x%x) (0x%x)", ecc, status, STATUS_FLAG_ECC_STATUS_MASK);
    }
}

bool SpiFlash::enable_writes() {
    enable();
    auto ok = simple_command(CMD_ENABLE_WRITE);
    disable();
    return ok;
}

bool SpiFlash::disable_writes() {
    enable();
    auto ok = simple_command(CMD_DISABLE_WRITE);
    disable();
    return ok;
}

bool SpiFlash::get_feature(uint8_t address, uint8_t *reg) {
    uint8_t command[] = { CMD_GET_FEATURE, address };
    enable();
    auto ok = transfer(command, sizeof(command), nullptr, reg, 1);
    disable();
    return ok;
}

bool SpiFlash::set_feature(uint8_t address, uint8_t value) {
    uint8_t command[] = { CMD_SET_FEATURE, address, value };
    enable();
    auto ok = transfer(command, sizeof(command), nullptr, nullptr, 0);
    disable();
    return ok;
}

uint8_t SpiFlash::read_status() {
    uint8_t status = 0x00;
    get_feature(CMD_REGISTER_3, &status);
    return status;
}

void SpiFlash::read_ecc_information() {
    uint8_t ecc[2] = { 0x00, 0x00 };
    get_feature(CMD_ECC_STATUS_0, &ecc[0]);
    get_feature(CMD_ECC_STATUS_1, &ecc[1]);
    loginfo("ecc(bfr): 0x%x 0x%x", ecc[0], ecc[0]);
}

bool SpiFlash::is_ready(bool ecc_check) {
    auto ok = false;

    enable();
    SPI.beginTransaction(SpiSettings);
    uint8_t command[] = { CMD_GET_FEATURE, CMD_REGISTER_3 };
    SPI.transfer(command[0]);
    SPI.transfer(command[1]);

    error_ = SpiFlashError::None;

    auto started = fk_uptime();
    auto status = 0x00;
    while (true) {
        status = SPI.transfer(0xff);

        if ((status & STATUS_FLAG_PROGRAM_FAIL) == STATUS_FLAG_PROGRAM_FAIL) {
            error_ = SpiFlashError::Program;
            logwarn("program failed");
            break;
        }
        if ((status & STATUS_FLAG_ERASE_FAIL) == STATUS_FLAG_ERASE_FAIL) {
            error_ = SpiFlashError::Erase;
            logwarn("erase failed");
            break;
        }

        if ((status & STATUS_FLAG_BUSY) != STATUS_FLAG_BUSY) {
            ok = true;
            break;
        }
        if (fk_uptime() - started > SpiFlashTimeoutMs) {
            break;
        }

        // This was an fk_delay, which would yield, also... is this too long?
        delay(1);
    }

    if (ecc_check) {
        auto ecc = (((status) & STATUS_FLAG_ECC_STATUS_MASK) >> STATUS_FLAG_ECC_STATUS_Pos);
        if (ecc > 0) {
            logwarn("ecc status: 0x%x (0x%x) (0x%x)", ecc, status, STATUS_FLAG_ECC_STATUS_MASK);
            read_ecc_information();
        }
    }

    SPI.endTransaction();
    disable();

    return ok;
}

void SpiFlash::enable() {
    // TODO: Mutex
    digitalWrite(cs_, LOW);
}

void SpiFlash::disable() {
    // TODO: Mutex
    digitalWrite(cs_, HIGH);
}

bool SpiFlash::simple_command(uint8_t command) {
    return transfer_command(command, nullptr, nullptr, 0);
}

bool SpiFlash::complex_command(uint8_t *command, uint32_t command_length) {
    return transfer(command, command_length, nullptr, nullptr, 0);
}

bool SpiFlash::read_command(uint8_t command, uint8_t *data, uint32_t data_length) {
    return transfer_command(command, nullptr, data, data_length);
}

bool SpiFlash::write_command(uint8_t command, uint8_t *data, uint32_t data_length) {
    return transfer_command(command, data, nullptr, data_length);
}

bool SpiFlash::transfer_command(uint8_t command, const uint8_t *data_w, uint8_t *data_r, uint32_t data_length) {
    return transfer(&command, 1, data_w, data_r, data_length);
}

bool SpiFlash::transfer(uint8_t *command, uint32_t command_length, const uint8_t *data_w, uint8_t *data_r, uint32_t data_length) {
    __disable_irq();
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
    __enable_irq();
    return true;
}

}

#endif
