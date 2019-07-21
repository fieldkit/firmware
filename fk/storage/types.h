#pragma once

#include "common.h"

namespace fk {

using BlockAddress = uint32_t;
using StorageAddress = uint32_t;

constexpr uint32_t InvalidAddress = (uint32_t)(-1);
constexpr uint32_t InvalidSize = (uint32_t)(-1);
constexpr uint32_t InvalidRecord = (uint32_t)(-1);
constexpr uint32_t InvalidVersion = (uint32_t)(-1);
constexpr uint32_t InvalidTimestamp  = (uint32_t)(-1);

constexpr uint32_t LastBlock = (uint32_t)(-1);
constexpr uint32_t LastRecord = (uint32_t)(-1);

constexpr int32_t NumberOfFiles = 4;

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

uint32_t hash_block(void *ptr, size_t size, Hash &hash);

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

    bool valid() {
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
    uint32_t overflow;
    FileHeader files[NumberOfFiles];
    Hash hash;

    void fill_hash() {
        hash_block(this, sizeof(BlockHeader) - sizeof(Hash), hash);
    }

    bool verify_hash() {
        Hash expected;
        hash_block(this, sizeof(BlockHeader) - sizeof(Hash), expected);
        return memcmp(expected.hash, hash.hash, sizeof(Hash)) == 0;
    }
};

struct BlockTail {
    uint32_t linked{ 0 };
    uint32_t reserved[3] = { 0xdeadbeef, 0xdeadbeef, 0xdeadbeef };
    Hash hash;

    void fill_hash() {
        hash_block(this, sizeof(BlockTail) - sizeof(Hash), hash);
    }

    bool verify_hash() {
        Hash expected;
        hash_block(this, sizeof(BlockTail) - sizeof(Hash), expected);
        return memcmp(expected.hash, hash.hash, sizeof(Hash)) == 0;
    }
};

struct RecordHeader {
    uint32_t size{ 0 };
    uint32_t record{ 0 };
    uint32_t crc{ 0 };
    uint32_t reserved[3] = { 0xdeadbeef, 0xdeadbeef, 0xdeadbeef };

    uint32_t sign();
    bool valid();
};

struct RecordTail {
    uint32_t size{ 0 };
    uint32_t reserved[3] = { 0xdeadbeef, 0xdeadbeef, 0xdeadbeef };
    Hash hash;
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

    SeekValue() {
    }

    SeekValue(uint32_t address, uint32_t record, uint32_t position) : address(address), record(record), position(position) {
    }

    bool valid() {
        return is_address_valid(address);
    }
};

}
