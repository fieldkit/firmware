#pragma once

#include <gtest/gtest.h>

#include <delimited_buffer.h>
#include <memory_sector_map.h>
#include <directory_chain.h>

#include "fake_data.h"
#include "geometry.h"
#include "memory_flash_memory.h"

using namespace phylum;

constexpr uint8_t ATTRIBUTE_ONE = 0x1;
constexpr uint8_t ATTRIBUTE_TWO = 0x2;

struct layout_256 {
    size_t sector_size{ 256 };
};

struct layout_4096 {
    size_t sector_size{ 4096 };
};

class FlashMemory {
private:
    size_t sector_size_;
    memory_flash_memory memory_{ sector_size_ };
    dhara_sector_map sectors_{ memory_ };
    sector_allocator allocator_{ sectors_ };
    bool formatted_{ false };
    bool initialized_{ false };

public:
    FlashMemory(size_t sector_size) : sector_size_(sector_size) {
    }

public:
    size_t sector_size() const {
        return sector_size_;
    }

    dhara_sector_map &sectors() {
        return sectors_;
    }

    sector_allocator &allocator() {
        return allocator_;
    }

public:
    void begin(bool force_create) {
        ASSERT_EQ(sectors_.begin(true), 0);
        initialized_ = true;
    }

    void clear() {
        sectors_.clear();
    }

    template<typename T>
    void sync(T fn) {
        fn();

        ASSERT_EQ(sectors_.sync(), 0);
    }

    template<typename T>
    void mounted(T fn) {
        if (formatted_) {
            ASSERT_EQ(sectors_.begin(false), 0);
        }
        else {
            ASSERT_EQ(sectors_.begin(!initialized_), 0);
            ASSERT_EQ(allocator_.begin(), 0);
            initialized_ = true;
        }

        directory_chain chain{ sectors_, allocator_, 0, simple_buffer{ sector_size() } };
        if (formatted_) {
            ASSERT_EQ(chain.mount(), 0);
        }
        else {
            ASSERT_EQ(chain.format(), 0);
            formatted_ = true;
        }

        fn(chain);

        ASSERT_EQ(sectors_.sync(), 0);
    }
};

class PhylumSuite : public ::testing::Test {
private:
    open_file_attribute attributes_[2];
    open_file_config file_cfg_;

public:
    open_file_config &file_cfg() {
        return file_cfg_;
    }

public:
    PhylumSuite() {
        attributes_[0] = open_file_attribute{ ATTRIBUTE_ONE, 4, nullptr, false };
        attributes_[1] = open_file_attribute{ ATTRIBUTE_TWO, 4, nullptr, false };
    }

    virtual ~PhylumSuite() {
    }

public:
    void SetUp() override {
        file_cfg_ = {
            .attributes = attributes_,
            .nattrs = 2,
        };
        for (auto &attr : attributes_) {
            attr.ptr = malloc(attr.size);
        }
    }

    void TearDown() override {
        for (auto &attr : attributes_) {
            free(attr.ptr);
        }
    }

};

class suppress_logs {
private:
    uint8_t saved_;

public:
    suppress_logs() {
        saved_ = log_get_level();
        log_configure_level(LogLevels::NONE);
    }

    virtual ~suppress_logs() {
        log_configure_level((LogLevels)saved_);
    }
};
