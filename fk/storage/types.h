#pragma once

#include "common.h"

namespace fk {

using FileNumber = uint8_t;
using BlockNumber = uint32_t;
using StorageAddress = uint32_t;
using StorageSize = uint32_t;
using StorageVersion = uint32_t;
using StorageTime = uint32_t;
using RecordNumber = uint32_t;
using RecordSize = uint32_t;

constexpr StorageAddress InvalidAddress = (uint32_t)(-1);
constexpr RecordNumber InvalidRecord = (uint32_t)(-1);
constexpr BlockNumber InvalidBlock = (uint32_t)(-1);
constexpr StorageSize InvalidSize = (uint32_t)(-1);
constexpr StorageVersion InvalidVersion = (uint32_t)(-1);
constexpr StorageTime InvalidTimestamp  = (uint32_t)(-1);

constexpr BlockNumber LastBlock = (uint32_t)(-1);
constexpr RecordNumber LastRecord = (uint32_t)(-1);

constexpr StorageSize SizeofBlockTail = 512;
constexpr StorageSize SizeofBlockHeader = 512;

constexpr FileNumber InvalidFile = (uint8_t)(-1);

constexpr uint32_t ReservedValue = 0xdeadbeef;

constexpr size_t NumberOfFiles = 4;

static inline bool is_block_valid(BlockNumber block) {
    return block != InvalidAddress;
}

static inline bool is_address_valid(StorageAddress address) {
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

    explicit BlockMagic() {
    }

    explicit BlockMagic(const char *k) {
        assert(k == MagicKey); // :)
        memcpy(data, k, sizeof(MagicKey));
    }

    static BlockMagic create_valid() {
        return BlockMagic{ MagicKey };
    }

    bool valid() const {
        return memcmp(data, MagicKey, sizeof(MagicKey)) == 0;
    }

    void fill() {
        memcpy(data, MagicKey, sizeof(MagicKey));
    }
};

struct FileHeader {
    StorageAddress tail{ InvalidAddress };
    RecordNumber record{ 0 };
    StorageSize size{ 0 };
    StorageVersion version{ 0 };

    FileHeader() {
    }

    FileHeader(StorageAddress tail, RecordNumber record, StorageSize size, StorageVersion version)
        : tail(tail), record(record), size(size), version(version) {
    }
};

struct BlockHeader {
    BlockMagic magic;
    StorageTime timestamp;
    FileNumber file;
    StorageVersion version;
    FileHeader files[NumberOfFiles];
    Hash hash;

    void fill_hash();

    bool verify_hash() const;

    bool valid() const;
};

struct BlockTail {
    StorageSize bytes_in_block{ 0 };
    RecordNumber records_in_block{ 0 };
    StorageAddress linked{ 0 };
    StorageAddress block_tail{ 0 };
    uint32_t reserved[2] = { ReservedValue, ReservedValue };
    Hash hash;

    void fill_hash();

    bool verify_hash();
};

struct RecordHeader {
    StorageSize size{ 0 };
    RecordNumber record{ 0 };
    StorageAddress previous{ InvalidAddress };
    uint32_t reserved[2] = { ReservedValue, ReservedValue };
    uint32_t crc{ 0 };

    uint32_t sign();
    bool valid();
};

struct RecordTail {
    RecordSize size{ 0 };
    uint32_t reserved[2] = { ReservedValue, ReservedValue };
    Hash hash;
    uint32_t crc{ 0 };

    uint32_t sign();
    bool valid();
};

struct SeekSettings {
    FileNumber file{ 0 };
    RecordNumber record{ 0 };

    SeekSettings() {
    }

    SeekSettings(FileNumber file, RecordNumber record)
        : file(file), record(record) {
    }

    static SeekSettings end_of(FileNumber file);
};

struct SeekValue {
    StorageAddress address{ InvalidAddress };
    RecordNumber record{ InvalidRecord };
    StorageSize position{ InvalidSize };
    BlockNumber block{ InvalidBlock };
    StorageTime timestamp{ InvalidTimestamp };
    StorageAddress record_address{ InvalidAddress };
    StorageSize bytes_in_block{ 0 };
    RecordNumber records_in_block{ 0 };

    SeekValue() {
    }

    SeekValue(StorageAddress address, RecordNumber record, StorageSize position, BlockNumber block, StorageTime timestamp, StorageAddress record_address, StorageSize bytes_in_block, RecordNumber records_in_block)
        : address(address), record(record), position(position), block(block),
          timestamp(timestamp), record_address(record_address), bytes_in_block(bytes_in_block), records_in_block(records_in_block) {
    }

    bool valid() {
        return is_address_valid(address);
    }
};

}
