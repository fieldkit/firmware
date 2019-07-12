#pragma once

#include "common.h"
#include "hal/hal.h"

#include <fk-data-protocol.h>

namespace fk {

using block_index_t = uint32_t;
using address_t = uint32_t;

constexpr uint32_t InvalidAddress = (uint32_t)(-1);
constexpr uint32_t InvalidSize = (uint32_t)(-1);
constexpr uint32_t LastBlock = (uint32_t)(-1);

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

struct Hash {
    constexpr static int32_t Length = 32;

    uint8_t hash[Length];
};

uint32_t hash_block(void *ptr, size_t size, Hash &hash);

struct FileHeader {
    uint32_t tail{ InvalidAddress };
    uint32_t sequence{ 0 };
    uint32_t size{ 0 };
    uint32_t version{ 0 };

    FileHeader() {
    }

    FileHeader(uint32_t tail, uint32_t sequence, uint32_t size, uint32_t version)
        : tail(tail), sequence(sequence), size(size), version(version) {
    }
};

struct OpenedFile {
    uint32_t tail{ InvalidAddress };
    uint32_t sequence{ 0 };
    uint32_t size{ 0 };
    uint32_t version{ 0 };

    OpenedFile() {
    }

    OpenedFile(uint32_t tail, uint32_t sequence, uint32_t size, uint32_t version)
        : tail(tail), sequence(sequence), size(size), version(version) {
    }
};

constexpr int32_t NumberOfFiles = 4;

struct BlockHeader {
    BlockMagic magic;
    uint32_t block_file;
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

struct SeekSettings {
    uint8_t file{ 0 };
    uint32_t starting{ 0 };
    uint32_t desired_record{ 0 };

    SeekSettings() {
    }

    SeekSettings(uint8_t file, uint32_t starting, uint32_t desired_record)
        : file(file), starting(starting), desired_record(desired_record) {
    }
};

struct BlockTail {
    uint32_t linked;
    uint32_t reserved[3];
};

struct RecordHead {
    uint32_t size;
    uint32_t sequence;
    uint32_t reserved[2];
};

struct RecordTail {
    Hash hash;
};

class Storage {
private:
    DataMemory *memory_;
    OpenedFile files_[NumberOfFiles];
    uint32_t free_block_{ 0 };

public:
    Storage(DataMemory *memory);

public:
    bool begin();
    bool clear();

public:
    bool append(uint8_t file, uint8_t *record, size_t size);
    bool append(uint8_t file, fk_data_DataRecord *record);
    bool seek(SeekSettings settings);

public:
    uint32_t tail(uint8_t file) const {
        return files_[file].tail;
    }

    uint32_t sequence(uint8_t file) const {
        return files_[file].sequence;
    }

    uint32_t size(uint8_t file) const {
        return files_[file].size;
    }

private:
    uint32_t allocate(uint8_t file);

};

}
