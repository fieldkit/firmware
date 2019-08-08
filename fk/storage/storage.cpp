#include "storage/storage.h"
#include "storage/progress.h"

#include <phylum/crc.h>

namespace fk {

FK_DECLARE_LOGGER("storage");

constexpr char BlockMagic::MagicKey[];

uint32_t hash_block(void const *ptr, size_t size, Hash &hash) {
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

void BlockHeader::fill_hash() {
    hash_block(this, sizeof(BlockHeader) - sizeof(Hash), hash);
}

bool BlockHeader::verify_hash() const {
    Hash expected;
    hash_block(this, sizeof(BlockHeader) - sizeof(Hash), expected);
    return memcmp(expected.hash, hash.hash, sizeof(Hash)) == 0;
}

bool BlockHeader::valid() const {
    return version != InvalidVersion && magic.valid() && verify_hash();
}

void BlockTail::fill_hash() {
    hash_block(this, sizeof(BlockTail) - sizeof(Hash), hash);
}

bool BlockTail::verify_hash() {
    Hash expected;
    hash_block(this, sizeof(BlockTail) - sizeof(Hash), expected);
    return memcmp(expected.hash, hash.hash, sizeof(Hash)) == 0;
}

uint32_t RecordHeader::sign() {
    return phylum::crc32_checksum((uint8_t *)this, sizeof(RecordHeader) - sizeof(uint32_t));
}

bool RecordHeader::valid() {
    return sign() == crc;
}

SeekSettings SeekSettings::end_of(uint8_t file) {
    return SeekSettings{ file, LastRecord };
}

Storage::Storage(DataMemory *memory) : memory_(memory) {
}

Storage::~Storage() {
    if (memory_ != nullptr) {
        memory_->flush();
    }
}

bool Storage::begin() {
    auto g = memory_->geometry();

    for (auto i = 0; i < NumberOfFiles; ++i) {
        files_[i] = { };
    }

    // Look for the first available free block.
    auto had_valid_blocks = false;
    auto range = BlockRange{ 0, g.nblocks };
    while (!range.empty()) {
        BlockHeader block_header;
        auto address = range.middle_block() * g.block_size;
        if (!memory_->read(address, (uint8_t *)&block_header, sizeof(block_header))) {
            logerror("[?] read failed 0x%06x", address);
            return false;
        }

        if (block_header.valid()) {
            logtrace("[%d] found valid block (0x%06x)", block_header.file, address);

            FK_ASSERT(block_header.verify_hash());

            for (auto i = 0; i < NumberOfFiles; ++i) {
                files_[i] = block_header.files[i];
            }

            timestamp_ = block_header.timestamp;
            version_ = block_header.version;
            range = range.second_half();
            had_valid_blocks = true;

            loginfo("0x%06x found version %d", address, version_);
        }
        else {
            logtrace("[?] invalid block (0x%06x)", address);

            range = range.first_half();
        }
    }

    if (!had_valid_blocks) {
        return false;
    }

    free_block_ = range.start;

    // Make sure our header records are fully up to date by seeking to the end
    // of each file.
    for (auto file = (uint8_t)0; file < NumberOfFiles; ++file) {
        if (is_address_valid(files_[file].tail)) {
            auto sv = seek(SeekSettings::end_of(file));
            if (!sv.valid()) {
                return false;
            }

            files_[file].tail = sv.address;
            files_[file].size = sv.position;
            files_[file].record = sv.record;

            if (sv.block >= free_block_) {
                free_block_ = sv.block + 1;
            }

            if (sv.timestamp > timestamp_) {
                timestamp_ = sv.timestamp;
            }
        }
    }

    logtrace("opened (block = %d)", free_block_);

    return true;
}

bool Storage::clear() {
    auto g = memory_->geometry();

    auto range = BlockRange{ 0, g.nblocks };
    while (!range.empty()) {
        auto address = range.middle_block() * g.block_size;
        if (!memory_->erase_block(address)) {
            logerror("erase block 0x%06x failed", address);
            return false;
        }
        range = range.first_half();
    }

    for (auto i = 0; i < NumberOfFiles; ++i) {
        files_[i] = { };
    }

    free_block_ = 0;
    timestamp_ = 0;
    version_ = fk_random_i32(0, INT32_MAX);

    return true;
}

uint32_t Storage::allocate(uint8_t file, uint32_t overflow, uint32_t previous_tail_address) {
    auto g = memory_->geometry();
    auto address = InvalidAddress;

    // Find a good block.
    for (auto i = 0; i < StorageAvailableBlockLookAhead; ++i) {
        address = free_block_ * g.block_size;

        uint8_t buffer[32];
        if (memory_->read(address, buffer, sizeof(buffer)) != sizeof(buffer)) {
            logwarn("allocate: read failed");
            continue;
        }

        // This is the bad block indicator, creative.
        if (buffer[0] != 0x00) {
            break;
        }

        logwarn("[%d] allocating ignoring bad block: %d", file, free_block_);

        free_block_++;
        address = InvalidAddress;
    }

    FK_ASSERT(g.is_address_valid(address));

    // Erase new block and write header.
    if (!memory_->erase_block(address)) {
        logerror("allocate: erase failed");
        return InvalidAddress;
    }

    logdebug("[%d] allocating block #%d (0x%06x) (%d) (#%d) (%d bytes)",
             file, free_block_, address, overflow, files_[file].record, files_[file].size);

    timestamp_++;
    free_block_++;

    auto after_header = address + sizeof(BlockHeader);

    // Do this before writing header so the new value gets written.
    files_[file].tail = after_header;

    BlockHeader block_header;
    block_header.magic.fill();
    block_header.file = file;
    block_header.timestamp = timestamp_;
    block_header.version = version_;
    block_header.overflow = overflow;
    for (auto i = 0; i < NumberOfFiles; ++i) {
        block_header.files[i] = files_[i];
    }
    block_header.fill_hash();

    FK_ASSERT(version_ > 0 && version_ != InvalidVersion);

    if (memory_->write(address, (uint8_t *)&block_header, sizeof(BlockHeader)) != sizeof(BlockHeader)) {
        logerror("allocate: write header failed");
        return InvalidAddress;
    }

    // We have a good new block, so link the previous block to the new one.
    if (is_address_valid(previous_tail_address)) {
        BlockTail block_tail;
        block_tail.linked = address;
        block_tail.fill_hash();
        if (memory_->write(previous_tail_address, (uint8_t *)&block_tail, sizeof(BlockTail)) != sizeof(BlockTail)) {
            logerror("allocate: write tail failed");
            return 0;
        }

        logtrace("[%d] 0x%06x btail", file, previous_tail_address);
    }

    return after_header;
}

SeekValue Storage::seek(SeekSettings settings) {
    SequentialMemory memory{ memory_ };
    auto g = memory_->geometry();
    auto timestamp = (uint32_t)0;

    verify_opened();

    logtrace("[%d] seeking #%" PRIu32, settings.file, settings.record);

    // Binary search for the block to start with.
    BlockHeader file_block_header;
    file_block_header.timestamp = 0;
    auto range = BlockRange{ 0, g.nblocks };
    while (!range.empty()) {
        BlockHeader block_header;
        auto address = range.middle_block() * g.block_size;
        if (!memory_->read(address, (uint8_t *)&block_header, sizeof(block_header))) {
            logerror("[%d] read failed 0x%06x", settings.file, address);
            return { };
        }

        if (block_header.valid()) {
            auto &bfh = block_header.files[settings.file];
            logtrace("[%d] found valid block (0x%06x) (%d)", block_header.file, address, bfh.size);

            if (block_header.timestamp > timestamp) {
                timestamp = block_header.timestamp;
            }

            if (settings.record != InvalidRecord) {
                if (bfh.record > settings.record) {
                    range = range.first_half();
                }
                else {
                    range = range.second_half();
                }
            }
            else {
                range = range.second_half();
            }

            memcpy(&file_block_header, &block_header, sizeof(BlockHeader));
        }
        else {
            logverbose("[?] invalid block (0x%06x)", address);

            // Search earlier in the range for a valid block.
            range = range.first_half();
        }
    }

    // If at the start of the block, bump.
    auto &fh = file_block_header.files[settings.file];
    auto address = fh.tail;
    auto block = address / g.block_size;
    auto position = fh.size;
    auto record = (uint32_t)0;

    if (!is_address_valid(address)) {
        logwarn("[%d] no valid files", settings.file);
        return SeekValue{ };
    }

    logtrace("[%d] 0x%06x seeking #% " PRIu32 " (%d) from blk #%" PRIu32 " (bsz = %d bytes)", settings.file, address, settings.record, position, fh.tail / g.block_size, fh.size);

    while (true) {
        auto record_head = RecordHeader{};

        // If at the start of the block, bump.
        if (g.start_of_block(address)) {
            address += sizeof(BlockHeader);
        }

        if (memory.read(address, (uint8_t *)&record_head, sizeof(record_head)) != sizeof(record_head)) {
            return SeekValue{ };
        }

        // Is there a valid record here?
        if (record_head.size == 0 || record_head.size == InvalidSize || !record_head.valid()) {
            logtrace("[%d] 0x%06x invalid head", settings.file, address);
            break;
        }

        // Is this the record they're looking for?
        if (settings.record != InvalidRecord && record_head.record == settings.record) {
            logverbose("[%d] 0x%06x found record #%" PRIu32, settings.file, address, settings.record);
            break;
        }
        if (settings.record < record_head.record) {
            logverbose("[%d] 0x%06x found nearby record #%" PRIu32, settings.file, address, settings.record);
            break;
        }

        // Keep track of maximum record for the file.
        if (record_head.record >= record) {
            record = record_head.record + 1;
        }

        auto record_length = sizeof(RecordHeader) + record_head.size + sizeof(RecordTail);

        logverbose("[%d] 0x%06x seeking %4d/%4d (#%" PRIu32 ")", settings.file, address, record_length, record_head.size, record_head.record);

        // Skip over the record head, the actual record, and the tail (hash)
        FK_ASSERT(!g.spans_block(address, record_length));
        address += record_length;

        // Update size of the data we've scanned through.
        position += record_head.size;
    }

    logdebug("[%d] 0x%06x seeking #%" PRIu32 " done (#% " PRIu32 ") (%d bytes) (%d in block)",
             settings.file, address, settings.record,
             record, position, position - fh.size);

    return SeekValue{ address, record, position, block, timestamp };
}

File Storage::file(uint8_t file) {
    verify_opened();

    return File{ this, file, files_[file] };
}

uint32_t Storage::fsck() {
    constexpr static size_t BufferSize = 1024;

    verify_opened();

    loginfo("fsck: begin");

    auto file = this->file(0);

    if (!file.seek(LastRecord)) {
        logwarn("seek eof failed");
        return 0;
    }

    auto buffer = (uint8_t *)malloc(BufferSize);
    auto tracker = ProgressTracker{ "fsck", "", file.size() };

    FK_ASSERT(file.seek(0));

    while (tracker.busy()) {
        auto to_read = std::min<size_t>(BufferSize, tracker.remaining_bytes());
        auto nread = file.read(buffer, to_read);
        if (nread != to_read) {
            logwarn("fsck: (%d != %d)", nread, to_read);
        }
        tracker.update(nread);
    }

    free(buffer);

    loginfo("fsck: done (%dms)", tracker.elapsed());

    return 0;
}

void Storage::verify_opened() {
    FK_ASSERT(free_block_ != InvalidBlock);
}

}
