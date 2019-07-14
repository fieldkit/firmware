#include "storage.h"

#include <phylum/blake2b.h>

namespace fk {

FK_DECLARE_LOGGER("storage");

constexpr char BlockMagic::MagicKey[];

/*
  char buffer[64];
  bytes_to_hex_string(buffer, sizeof(buffer), hash.hash, sizeof(hash.hash));
  loginfo("%s", buffer);
*/

uint32_t hash_block(void *ptr, size_t size, Hash &hash) {
    BLAKE2b b2b;
    b2b.reset(Hash::Length);
    b2b.update(ptr, size);
    b2b.finalize(&hash.hash, Hash::Length);
    return 32;
}

struct BlockRange {
    uint32_t start;
    uint32_t nblocks;

    uint32_t middle_block() const {
        return start + (nblocks / 2);
    }

    BlockRange first_half() const {
        return { start, (nblocks / 2) };
    }

    BlockRange second_half() const {
        return { start + (nblocks / 2), (nblocks / 2) };
    }

    bool empty() const {
        return nblocks == 0;
    }
};

Storage::Storage(DataMemory *memory) : memory_(memory) {
}

static bool is_block_valid(uint32_t block) {
    return block != InvalidAddress;
}

static bool is_address_valid(uint32_t address) {
    return address != InvalidAddress;
}

bool Storage::seek(SeekSettings settings) {
    SequentialMemory memory{ memory_ };
    auto g = memory_->geometry();

    auto &fh = files_[settings.file];
    auto address = is_block_valid(settings.starting) ? settings.starting : fh.tail;
    auto record_head = RecordHead{};

    FK_ASSERT(is_address_valid(address));

    logtrace("[%d] seeking", settings.file);

    // Adjust start position if we're at the start of a block.
    if (address % g.block_size == 0) {
        address += sizeof(BlockHeader);
    }

    while (true) {
        if (memory.read(address, (uint8_t *)&record_head, sizeof(record_head)) != sizeof(record_head)) {
            return false;
        }

        if (record_head.size == 0 || record_head.size == InvalidSize) {
            files_[settings.file].tail = address;
            break;
        }

        // Keep track of maximum sequence for the file.
        if (record_head.sequence > fh.sequence) {
            fh.sequence = record_head.sequence + 1;
        }

        fh.size += record_head.size;

        // Skip over the record head, the actual record, and the tail (hash)
        address += sizeof(RecordHead) + record_head.size + sizeof(RecordTail);
    }

    return true;
}

bool Storage::begin() {
    auto g = memory_->geometry();

    for (auto i = 0; i < NumberOfFiles; ++i) {
        files_[i] = { };
    }

    logtrace("opening");

    BlockHeader block_header;
    auto range = BlockRange{ 0, g.nblocks };
    while (!range.empty()) {
        auto address = range.middle_block() * g.block_size;
        if (!memory_->read(address, (uint8_t *)&block_header, sizeof(block_header))) {
            return false;
        }

        if (block_header.magic.valid()) {
            logtrace("[%d] found valid block (0x%06x)", block_header.block_file, address);

            for (auto i = 0; i < NumberOfFiles; ++i) {
                files_[i] = OpenedFile{
                    block_header.files[i].tail,
                    block_header.files[i].sequence,
                    block_header.files[i].size,
                    block_header.files[i].version,
                };
            }

            range = range.second_half();
        }
        else {
            range = range.first_half();
        }
    }

    for (auto file = 0; file < NumberOfFiles; ++file) {
        if (is_address_valid(files_[file].tail)) {
            auto settings = SeekSettings{ (uint8_t)file, 0, LastBlock };
            if (!seek(settings)) {
                return false;
            }
        }
    }

    free_block_ = range.start;

    logtrace("opened!");

    return true;
}

bool Storage::clear() {
    auto g = memory_->geometry();

    loginfo("formatting");

    auto range = BlockRange{ 0, g.nblocks };
    while (!range.empty()) {
        auto address = range.middle_block() * g.block_size;
        if (!memory_->erase_block(address)) {
            return false;
        }
        range = range.first_half();
    }

    free_block_ = 0;

    return true;
}

uint32_t Storage::allocate(uint8_t file) {
    auto g = memory_->geometry();

    loginfo("[%d] allocating (%d)", file, free_block_);

    auto address = free_block_ * g.block_size;
    free_block_++;

    files_[file].tail = address + sizeof(BlockHeader);

    BlockHeader block_header;
    block_header.magic.fill();
    block_header.block_file = file;
    for (auto i = 0; i < NumberOfFiles; ++i) {
        block_header.files[i] = FileHeader{
            files_[i].tail,
            files_[i].sequence,
            files_[i].size,
            files_[i].version,
        };
    }
    block_header.fill_hash();

    if (!memory_->write(address, (uint8_t *)&block_header, sizeof(BlockHeader))) {
        return InvalidAddress;
    }

    return address + sizeof(BlockHeader);
}

bool Storage::append(uint8_t file, uint8_t *record, uint32_t size) {
    FK_ASSERT(file < NumberOfFiles);

    SequentialMemory memory{ memory_ };
    auto g = memory_->geometry();
    auto &fh = files_[file];
    BLAKE2b b2b;
    b2b.reset(Hash::Length);

    logtrace("[%d] appending %d @ 0x%06x (%d)", file, fh.sequence, fh.tail, size);

    auto available = g.remaining_in_block(fh.tail);
    auto wrote = (uint32_t)0;
    auto header = false;
    auto footer = false;
    while (!header || wrote < size || !footer) {
        if (!is_address_valid(fh.tail) || (!header && available < sizeof(RecordHead)) || available == 0 || (!footer && available < sizeof(RecordTail))) {
            fh.tail = allocate(file);
            available = g.remaining_in_block(fh.tail);
        }

        if (!header) {
            RecordHead record_header = { 0 };
            record_header.size = size;
            record_header.sequence = fh.sequence++;
            if (memory.write(fh.tail, (uint8_t *)&record_header, sizeof(record_header)) != sizeof(record_header)) {
                return false;
            }

            fh.tail += sizeof(record_header);
            available -= sizeof(record_header);
            header = true;

            b2b.update(&header, sizeof(header));
        }
        else if (wrote < size) {
            auto writing = std::min(available, size);
            if (memory.write(fh.tail, (uint8_t *)record + wrote, writing) != size) {
                return false;
            }

            fh.tail += writing;
            wrote += writing;
            available -= writing;
        }
        else {
            break;
        }
    }

    uint8_t hash[Hash::Length];
    b2b.update(record, size);
    b2b.finalize(hash, sizeof(hash));
    if (memory.write(fh.tail, (uint8_t *)hash, sizeof(hash)) != sizeof(hash)) {
        return false;
    }

    fh.tail += sizeof(hash);
    fh.size += size;

    return true;
}

bool Storage::append(uint8_t file, fk_data_DataRecord *record) {
    size_t size = 0;
    auto fields = fk_data_DataRecord_fields;
    if (!pb_get_encoded_size(&size, fields, record)) {
        return false;
    }

    return true;
}

}
