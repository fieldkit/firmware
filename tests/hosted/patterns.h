#pragma once

#include "common.h"
#include "storage/storage.h"

using namespace fk;

struct StaticPattern {
    uint8_t data[256];

    StaticPattern(uint8_t value = 0xcc) {
        memset(data, value, sizeof(data));
    }

    void write(File &file, size_t size) {
        size_t counter = 0;
        size_t bytes_wrote = 0;
        while (bytes_wrote < size) {
            ASSERT_EQ(file.write((uint8_t *)data, sizeof(data)), sizeof(data));

            bytes_wrote += sizeof(data);
            counter++;
        }
    }

    void verify(File &file, size_t size) {
        ASSERT_TRUE(file.seek(0));

        uint8_t expected[256];
        memset(expected, 0xcc, sizeof(expected));

        size_t bytes_read = 0;
        while (bytes_read < size) {
            verify_record(file);

            bytes_read += sizeof(data);
        }
    }

    void verify_record(File &file) {
        memset(data, 0x00, sizeof(data));
        ASSERT_EQ(file.read(data, sizeof(data)), sizeof(data));

        uint8_t expected[256];
        memset(expected, 0xcc, sizeof(expected));
        ASSERT_EQ(memcmp(expected, data, sizeof(data)), 0);
    }
};

struct SequentialPattern {
    uint8_t data[256];

    void write(File &file, size_t size) {
        size_t counter = 0;
        size_t bytes_wrote = 0;
        while (bytes_wrote < size) {
            memset(data, counter, sizeof(data));
            ASSERT_EQ(file.write(data, sizeof(data)), sizeof(data));

            bytes_wrote += sizeof(data);
            counter++;
        }
    }

    void verify_record(File &file, uint8_t value) {
        ASSERT_EQ(file.read(data, sizeof(data)), sizeof(data));

        uint8_t expected[256];
        memset(expected, value, sizeof(expected));

        auto equal = memcmp(expected, data, sizeof(data));
        if (equal != 0) {
            fk_dump_memory(data, sizeof(data));
            ASSERT_EQ(memcmp(expected, data, sizeof(data)), 0);
        }
    }
};
