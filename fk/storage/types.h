#pragma once

#include "common.h"

namespace fk {

using BlockAddress = uint32_t;
using StorageAddress = uint32_t;

constexpr uint32_t InvalidAddress = (uint32_t)(-1);
constexpr uint32_t InvalidSize = (uint32_t)(-1);
constexpr uint32_t InvalidRecord = (uint32_t)(-1);
constexpr uint32_t InvalidBlock = (uint32_t)(-1);
constexpr uint32_t InvalidVersion = (uint32_t)(-1);
constexpr uint32_t InvalidTimestamp  = (uint32_t)(-1);

constexpr uint32_t LastBlock = (uint32_t)(-1);
constexpr uint32_t LastRecord = (uint32_t)(-1);

constexpr int32_t NumberOfFiles = 4;

constexpr uint32_t SizeofBlockTail = 512;
constexpr uint32_t SizeofBlockHeader = 512;

static inline bool is_block_valid(uint32_t block) {
    return block != InvalidAddress;
}

static inline bool is_address_valid(uint32_t address) {
    return address != InvalidAddress;
}

struct Hash {
    constexpr static int32_t Length = 32;

    uint8_t hash[Length];
};

uint32_t hash_block(void const *ptr, size_t size, Hash &hash);

struct BlockMagic {
    static constexpr char MagicKey[] = "phylum0";

    uint8_t data[sizeof(MagicKey)] = { 0 };

    BlockMagic() {
    }

    BlockMagic(const char *k) {
        assert(k == MagicKey); // :)
        memcpy(data, k, sizeof(MagicKey));
    }

    static BlockMagic create_valid() {
        return { MagicKey };
    }

    bool valid() const {
        return memcmp(data, MagicKey, sizeof(MagicKey)) == 0;
    }

    void fill() {
        memcpy(data, MagicKey, sizeof(MagicKey));
    }
};

struct FileHeader {
    uint32_t tail{ InvalidAddress };
    uint32_t record{ 0 };
    uint32_t size{ 0 };
    uint32_t version{ 0 };

    FileHeader() {
    }

    FileHeader(uint32_t tail, uint32_t record, uint32_t size, uint32_t version)
        : tail(tail), record(record), size(size), version(version) {
    }
};

struct BlockHeader {
    BlockMagic magic;
    uint32_t timestamp;
    uint32_t file;
    uint32_t version;
    FileHeader files[NumberOfFiles];
    Hash hash;

    void fill_hash();

    bool verify_hash() const;

    bool valid() const;
};

struct BlockTail {
    uint32_t bytes_in_block{ 0 };
    uint32_t records_in_block{ 0 };
    uint32_t linked{ 0 };
    uint32_t block_tail{ 0 };
    uint32_t reserved[2] = { 0xdeadbeef, 0xdeadbeef };
    Hash hash;

    void fill_hash();

    bool verify_hash();
};

struct RecordHeader {
    uint32_t size{ 0 };
    uint32_t record{ 0 };
    uint32_t reserved[3] = { 0xdeadbeef, 0xdeadbeef, 0xdeadbeef };
    uint32_t crc{ 0 };

    uint32_t sign();
    bool valid();
};

struct RecordTail {
    uint32_t size{ 0 };
    uint32_t reserved[3] = { 0xdeadbeef, 0xdeadbeef, 0xdeadbeef };
    Hash hash;

    bool valid();
};

struct SeekSettings {
    uint8_t file{ 0 };
    uint32_t record{ 0 };

    SeekSettings() {
    }

    SeekSettings(uint8_t file, uint32_t record)
        : file(file), record(record) {
    }

    static SeekSettings end_of(uint8_t file);
};

struct SeekValue {
    uint32_t address{ InvalidAddress };
    uint32_t record{ InvalidRecord };
    uint32_t position{ InvalidAddress };
    uint32_t block{ InvalidBlock };
    uint32_t timestamp{ InvalidTimestamp };
    uint32_t record_address{ InvalidAddress };
    uint32_t bytes_in_block{ 0 };
    uint32_t records_in_block{ 0 };

    SeekValue() {
    }

    SeekValue(uint32_t address, uint32_t record, uint32_t position, uint32_t block, uint32_t timestamp, uint32_t record_address, uint32_t bytes_in_block, uint32_t records_in_block)
        : address(address), record(record), position(position), block(block),
          timestamp(timestamp), record_address(record_address), bytes_in_block(bytes_in_block), records_in_block(records_in_block) {
    }

    bool valid() {
        return is_address_valid(address);
    }
};

}
