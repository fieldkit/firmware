#pragma once

#include "common.h"
#include "io.h"
#include "hal/memory.h"

namespace fk {

class FlashMemory {
public:
    virtual int32_t read(uint32_t address, uint8_t *data, size_t size) = 0;
    virtual int32_t write(uint32_t address, uint8_t const *data, size_t size) = 0;
    virtual int32_t erase(uint32_t address, size_t size) = 0;
    virtual const char *name() = 0;
    virtual uint32_t flash_to_cpu(uint32_t address) {
        return address;
    }
};

class DataMemoryFlash : public FlashMemory {
private:
    DataMemory *data_;

public:
    DataMemoryFlash(DataMemory *data);
    virtual ~DataMemoryFlash();

public:
    int32_t read(uint32_t address, uint8_t *data, size_t size) override {
        return data_->read(address, data, size);
    }

    int32_t write(uint32_t address, uint8_t const *data, size_t size) override {
        return data_->write(address, data, size);
    }

    int32_t erase(uint32_t address, size_t size) override {
        return data_->erase(address, size);
    }

    uint32_t flash_to_cpu(uint32_t address) override {
        return address + 0x040000000;
    }

    const char *name() override {
        return "qspi";
    }

};

class FlashWriter : public Writer {
private:
    FlashMemory *memory_{ nullptr };
    uint32_t address_{ 0x0 };
    uint32_t erased_{ 0x0 };

public:
    FlashWriter(FlashMemory *memory, uint32_t address);
    virtual ~FlashWriter();

public:
    int32_t write(uint8_t const *buffer, size_t size) override;

};

class Flash : public FlashMemory {
private:
    uint32_t page_size_{ 0 };
    uint32_t total_pages_{ 0 };

public:
    Flash() { }

public:
    bool initialize();

public:
    int32_t read(uint32_t address, uint8_t *data, size_t size) override;
    int32_t write(uint32_t address, uint8_t const *data, size_t size) override;
    int32_t erase(uint32_t address, size_t size) override;
    const char *name() override {
        return "samd51";
    }

public:
    uint32_t page_size() const {
        return page_size_;
    }

    uint32_t total_pages() const {
        return total_pages_;
    }
};

Flash *get_flash();

}
