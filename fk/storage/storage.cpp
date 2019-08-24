#include <algorithm>
#include <phylum/crc.h>

#include "platform.h"
#include "hal/random.h"
#include "storage/storage.h"
#include "storage/progress.h"

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
    return size > 0 && size != InvalidSize && sign() == crc;
}

SeekSettings SeekSettings::end_of(uint8_t file) {
    return SeekSettings{ file, LastRecord };
}

Storage::Storage(DataMemory *memory) : memory_(memory) {
}

Storage::~Storage() {
    if (memory_ != nullptr) {
        if (!memory_->flush()) {
            logerror("flush failed");
        }
    }
}

bool Storage::valid_block_header(BlockHeader &header) const {
    return header.valid() && (version_ == InvalidVersion || header.version == version_);
}

bool Storage::begin() {
    auto g = memory_->geometry();
    auto started = fk_uptime();

    free_block_ = 0;
    timestamp_ = 0;

    for (auto i = 0; i < NumberOfFiles; ++i) {
        files_[i] = { };
    }

    if (!memory_->begin()) {
        return false;
    }

    // Look for the first available free block.
    auto had_valid_blocks = false;
    auto range = BlockRange{ 0, g.nblocks };
    while (!range.empty()) {
        BlockHeader block_header;
        auto address = range.middle_block() * g.block_size;
        if (memory_->read(address, (uint8_t *)&block_header, sizeof(block_header)) != sizeof(block_header)) {
            logerror("[?] read failed " PRADDRESS, address);
            return false;
        }

        if (valid_block_header(block_header)) {
            logtrace("[%" PRIu32 "] valid block (" PRADDRESS ") (v = %" PRIu32 ") (ts = %" PRIu32 ")", block_header.file, address, block_header.version, block_header.timestamp);

            FK_ASSERT(block_header.verify_hash());

            for (auto i = 0; i < NumberOfFiles; ++i) {
                files_[i] = block_header.files[i];
            }

            timestamp_ = block_header.timestamp;
            version_ = block_header.version;
            range = range.second_half();
            had_valid_blocks = true;
        }
        else {
            logtrace("[?] invalid block (" PRADDRESS ")", address);
            range = range.first_half();
        }
    }

    if (!had_valid_blocks) {
        return false;
    }

    auto block = range.start;

    while (true) {
        auto head_address = (uint32_t)(g.block_size * block);
        auto tail_address = head_address + g.block_size - sizeof(BlockTail);

        BlockHeader block_header;
        if (memory_->read(head_address, (uint8_t *)&block_header, sizeof(block_header)) != sizeof(block_header)) {
            return 0;
        }

        if (!valid_block_header(block_header)) {
            break;
        }

        BlockTail block_tail;
        if (memory_->read(tail_address, (uint8_t *)&block_tail, sizeof(block_tail)) != sizeof(block_tail)) {
            return false;
        }

        if (!is_address_valid(block_tail.linked)) {
            block++;
            break;
        }

        FK_ASSERT(block != block_tail.linked);

        block = block_tail.linked;
    }

    free_block_ = block;

    logdebug("found end (block = %" PRIu32 ") (%" PRIu32 "ms)", free_block_, fk_uptime() - started);

    return true;
}

bool Storage::clear() {
    auto g = memory_->geometry();

    free_block_ = 0;
    timestamp_ = 0;
    version_ = fk_random_i32(0, INT32_MAX);

    for (auto i = 0; i < NumberOfFiles; ++i) {
        files_[i] = { };
    }

    if (!memory_->begin()) {
        return false;
    }

    auto range = BlockRange{ 0, g.nblocks };
    while (!range.empty()) {
        uint32_t address = range.middle_block() * g.block_size;
        logdebug("[" PRADDRESS "] erasing block", address);
        if (!memory_->erase_block(address)) {
            logerror("erase block " PRADDRESS " failed", address);
            return false;
        }
        range = range.first_half();
    }

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

        logwarn("[%d] allocating ignoring bad block: %" PRIu32, file, free_block_);

        free_block_++;
        address = InvalidAddress;
    }

    FK_ASSERT(g.is_address_valid(address));

    // Erase new block and write header.
    logdebug("[" PRADDRESS "] erasing block", address);
    if (!memory_->erase_block(address)) {
        logerror("allocate: erase failed");
        return InvalidAddress;
    }

    logdebug("[%d] allocating block #%" PRIu32 " (" PRADDRESS ") (%" PRIu32 ") (#%" PRIu32 ") (%" PRIu32 " bytes)",
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

        auto start_of_previous_block = previous_tail_address - (g.block_size - sizeof(BlockTail));
        FK_ASSERT(address != start_of_previous_block);

        if (memory_->write(previous_tail_address, (uint8_t *)&block_tail, sizeof(BlockTail)) != sizeof(BlockTail)) {
            logerror("allocate: write tail failed");
            return 0;
        }

        logtrace("[%d] " PRADDRESS " btail", file, previous_tail_address);
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
            logerror("[%d] read failed " PRADDRESS, settings.file, address);
            return SeekValue{ };
        }

        if (valid_block_header(block_header)) {
            auto &bfh = block_header.files[settings.file];
            logtrace("[%" PRIu32 "] valid block (" PRADDRESS ") (v = %" PRIu32 ") (ts = %" PRIu32 ") (%" PRIu32 ")", block_header.file, address, block_header.version, block_header.timestamp, bfh.size);

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
            logverbose("[?] invalid block (" PRADDRESS ")", address);

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
    auto record_address = (uint32_t)0;

    // If the address is invalid then this file is empty, nothing to be found,
    // this is success if they were looking for the last record of the file.
    if (!is_address_valid(address)) {
        return SeekValue{ };
    }

    logtrace("[%d] " PRADDRESS " seeking #%" PRIu32 " (%" PRIu32 ") from blk #%" PRIu32 " (bsz = %" PRIu32 " bytes)", settings.file, address, settings.record, position, fh.tail / g.block_size, fh.size);

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
        if (!record_head.valid()) {
            logtrace("[%d] " PRADDRESS " invalid head", settings.file, address);
            break;
        }

        // We've got a valid record header so let's remember this position.
        record_address = address;

        // Is this the record they're looking for, or is this after the record
        // they're looking for?
        if (settings.record != InvalidRecord) {
            if (record_head.record == settings.record) {
                logverbose("[%d] " PRADDRESS " found record #%" PRIu32, settings.file, address, settings.record);
                break;
            }
            if (settings.record < record_head.record) {
                logverbose("[%d] " PRADDRESS " found nearby record #%" PRIu32, settings.file, address, settings.record);
                break;
            }
        }

        // Keep track of maximum record for the file.
        if (record_head.record >= record) {
            record = record_head.record + 1;
        }

        auto record_length = (uint32_t)(sizeof(RecordHeader) + record_head.size + sizeof(RecordTail));

        logverbose("[%d] " PRADDRESS " seeking %4" PRIu32 "/%4" PRIu32 " (#%" PRIu32 ")",
                   settings.file, address, record_length, record_head.size, record_head.record);

        // Skip over the record head, the actual record, and the tail (hash)
        FK_ASSERT(!g.spans_block(address, record_length));
        address += record_length;

        // Update size of the data we've scanned through.
        position += record_head.size;
    }

    logdebug("[%d] " PRADDRESS " seeking #%" PRIu32 " done (#%" PRIu32 ") (%" PRIu32 " bytes) (%" PRIu32 " in block)",
             settings.file, address, settings.record,
             record, position, position - fh.size);

    return SeekValue{ address, record, position, block, timestamp, record_address };
}

File Storage::file(uint8_t file) {
    verify_opened();

    return File{ this, file };
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
        FK_ASSERT(nread == to_read);
        tracker.update(nread);
    }

    free(buffer);

    loginfo("fsck: done (%" PRIu32 "ms)", tracker.elapsed());

    return 0;
}

void Storage::verify_opened() const {
    FK_ASSERT(free_block_ != InvalidBlock);
}

}
