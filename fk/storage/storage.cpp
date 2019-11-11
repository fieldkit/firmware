#include <algorithm>
#include <phylum/crc.h>

#include "storage/storage.h"

#include "platform.h"
#include "hal/random.h"
#include "progress_tracker.h"

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

bool RecordTail::valid() {
    return size > 0 && size != InvalidSize;
}

SeekSettings SeekSettings::end_of(uint8_t file) {
    return SeekSettings{ file, LastRecord };
}

Storage::Storage(DataMemory *memory, bool read_only) : data_memory_(memory), memory_(memory), bad_blocks_(memory), read_only_(read_only) {
}

Storage::~Storage() {
    if (!memory_.flush()) {
        logerror("flush failed");
    }
}

bool Storage::valid_block_header(BlockHeader &header) const {
    return header.valid() && (version_ == InvalidVersion || header.version == version_);
}

bool Storage::begin() {
    auto g = memory_.geometry();
    auto started = fk_uptime();

    free_block_ = 0;
    timestamp_ = 0;

    for (auto i = 0; i < NumberOfFiles; ++i) {
        files_[i] = { };
    }

    if (!data_memory_->begin()) {
        return false;
    }

    // Look for the first available free block.
    auto had_valid_blocks = false;
    auto range = BlockRange{ 0, g.nblocks };
    while (!range.empty()) {
        BlockHeader block_header;
        auto address = range.middle_block() * g.block_size;
        if (memory_.read(address, (uint8_t *)&block_header, sizeof(block_header)) != sizeof(block_header)) {
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
        auto tail_address = head_address + g.block_size - SizeofBlockTail;

        BlockHeader block_header;
        if (memory_.read(head_address, (uint8_t *)&block_header, sizeof(block_header)) != sizeof(block_header)) {
            return 0;
        }

        if (!valid_block_header(block_header)) {
            break;
        }

        BlockTail block_tail;
        if (memory_.read(tail_address, (uint8_t *)&block_tail, sizeof(block_tail)) != sizeof(block_tail)) {
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

    for (auto i = 0; i < NumberOfFiles; ++i) {
        logdebug("[%d] header tail (" PRADDRESS ") (#%" PRIu32 ")", i, files_[i].tail, files_[i].record);
    }

    logdebug("found end (block = %" PRIu32 ") (%" PRIu32 "ms)", free_block_, fk_uptime() - started);

    return true;
}

bool Storage::clear() {
    auto g = memory_.geometry();

    free_block_ = 0;
    timestamp_ = 0;
    version_ = fk_random_i32(0, INT32_MAX);

    for (auto i = 0; i < NumberOfFiles; ++i) {
        files_[i] = { };
    }

    if (!data_memory_->begin()) {
        return false;
    }

    auto range = BlockRange{ 0, g.nblocks };
    while (!range.empty()) {
        uint32_t address = range.middle_block() * g.block_size;
        logdebug("[" PRADDRESS "] erasing block", address);
        if (!memory_.erase_block(address)) {
            // We just keep going, clearing earlier blocks. We'll
            // handle this block being bad during reads/seeks. Still
            // mark the block as bad though so that we can remember.
            logerror("erase block " PRADDRESS " failed", address);
            bad_blocks_.mark_address_as_bad(address);
        }
        range = range.first_half();
    }

    return true;
}

struct BadBlockFactoryCheck {
    static constexpr size_t BadBlockFactoryCheckSize = 32;
    uint8_t data[BadBlockFactoryCheckSize];

    bool is_bad() const {
        for (auto i = 0u; i < BadBlockFactoryCheckSize; ++i) {
            if (data[i] != 0) {
                return false;
            }
        }

        return true;
    }
};

uint32_t Storage::allocate(uint8_t file, uint32_t previous_tail_address, BlockTail &block_tail) {
    auto g = memory_.geometry();
    int32_t rv;

    verify_mutable();

    logdebug("allocating");

    // Find a good block.
    for (auto i = 0; i < StorageAvailableBlockLookAhead; ++i) {
        auto block = free_block_;
        auto address = block * g.block_size;

        free_block_++;

        auto bad = bad_blocks_.is_address_bad(address);
        if (bad) {
            logerror("[%d] allocating ignoring bad block: %" PRIu32 " (marked bad)", file, block);
            continue;
        }

        // Basically checking an arbitrary amount of data here.
        BadBlockFactoryCheck check;
        rv = memory_.read(address, check.data, sizeof(check.data));
        if (rv <= 0) {
            logerror("allocate: read failed");
            continue;
        }

        // This is the bad block indicator, creative.
        if (check.is_bad()) {
            logerror("[%d] allocating ignoring bad block: %" PRIu32 " (factory bad)", file, block);
            bad_blocks_.mark_address_as_bad(address);
            continue;
        }

        // Erase new block and write header.
        rv = memory_.erase_block(address);
        if (rv <= 0) {
            logerror("[%d] allocating ignoring bad block: %" PRIu32 " (erase failed)", file, block);
            bad_blocks_.mark_address_as_bad(address);
            continue;
        }

        loginfo("[%d] allocating block #%" PRIu32 " ts=%" PRIu32 " (" PRADDRESS ") (pta=" PRADDRESS ") (try #%d) (#%" PRIu32 ") (%" PRIu32 " bytes)",
                file, block, timestamp_, address, previous_tail_address, i, files_[file].record, files_[file].size);

        // First sector is the block header. We force write this to
        // ensure the block is good.
        auto first_record_address = address + g.sector_size();

        // Do this before writing header so the new value gets written.
        files_[file].tail = first_record_address;

        BlockHeader block_header;
        block_header.magic.fill();
        block_header.file = file;
        block_header.timestamp = timestamp_;
        block_header.version = version_;
        for (auto i = 0; i < NumberOfFiles; ++i) {
            block_header.files[i] = files_[i];
        }
        block_header.fill_hash();

        FK_ASSERT(version_ > 0 && version_ != InvalidVersion);

        rv = data_memory_->write(address, (uint8_t *)&block_header, sizeof(BlockHeader));
        if (rv <= 0) {
            logerror("allocate: write header failed");
            bad_blocks_.mark_address_as_bad(address);
            continue;
        }

        // We have a good new block, so link the previous block.
        bad = bad_blocks_.is_address_bad(previous_tail_address);
        if (!bad && is_address_valid(previous_tail_address)) {
            block_tail.linked = address;
            block_tail.fill_hash();

            auto start_of_previous_block = previous_tail_address - (g.block_size - SizeofBlockTail);
            FK_ASSERT(address != start_of_previous_block);

            rv = data_memory_->write(previous_tail_address, (uint8_t *)&block_tail, sizeof(BlockTail));
            if (rv <= 0) {
                logerror("allocate: write tail failed");
                return rv;
            }

            logdebug("[%d] " PRADDRESS " write btail linked(0x%" PRIx32 ") (%" PRIu32 " bib)",
                    file, previous_tail_address, address, block_tail.bytes_in_block);
        }

        timestamp_++;

        return first_record_address;
    }

    return 0;
}

SeekValue Storage::seek(SeekSettings settings) {
    auto g = memory_.geometry();
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
        auto rv = memory_.read(address, (uint8_t *)&block_header, sizeof(block_header));
        if (rv <= 0) {
            logerror("[%d] read failed " PRADDRESS, settings.file, address);
            return SeekValue{ };
        }

        if (valid_block_header(block_header)) {
            auto &bfh = block_header.files[settings.file];
            logtrace("[%" PRIu32 "] valid block (%d) (" PRADDRESS ") (v = %" PRIu32 ") (ts = %" PRIu32 ") (%" PRIu32 " bytes bob) (" PRADDRESS ")",
                     block_header.file, settings.file, address, block_header.version, block_header.timestamp, bfh.size, bfh.tail);

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
    auto record = (uint32_t)fh.record;
    auto record_address = 0u;
    auto bytes_in_block = 0u;
    auto records_in_block = 0u;

    // If the address is invalid then this file is empty, nothing to be found,
    // this is success if they were looking for the last record of the file.
    if (!is_address_valid(address)) {
        return SeekValue{ };
    }

    logdebug("[%d] " PRADDRESS " seeking #%" PRIu32 " (%" PRIu32 ") from blk #%" PRIu32 " (bsz = %" PRIu32 " bytes)",
             settings.file, address, settings.record, position, fh.tail / g.block_size, fh.size);

    while (true) {
        // If at the start of the block, bump.
        if (g.is_start_of_block(address)) {
            address += sizeof(BlockHeader);
        }

        RecordHeader record_head;
        auto rv = memory_.read(address, (uint8_t *)&record_head, sizeof(record_head));
        if (rv <= 0) {
            return SeekValue{ };
        }

        // Is there a valid record here?
        if (!record_head.valid()) {
            auto partial_aligned = g.partial_write_boundary_after(address);
            rv = memory_.read(partial_aligned, (uint8_t *)&record_head, sizeof(record_head));
            if (rv <= 0) {
                return SeekValue{ };
            }

            if (!record_head.valid()) {
                // This is here so that we can get the correct record_address. This
                // will typically happen when the file block header pointed directly
                // to the tail of the file, so we have the record number but
                // record_address is unset because we haven't scanned the block yet.
                // This isn't the most elegant.
                if (record_address == 0 && record > 0) {
                    auto previous_address = address;
                    address = g.start_of_block(previous_address);
                    logdebug("[%d] " PRADDRESS " invalid head (resume " PRADDRESS ")", settings.file, previous_address, address);
                    if (previous_address == address) {
                        return SeekValue{ };
                    }
                    continue;
                }

                logtrace("[%d] " PRADDRESS " invalid head", settings.file, address);
                break;
            }

            address = partial_aligned;
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
        if (address > fh.tail) {
            position += record_head.size;
            bytes_in_block += record_head.size;
            records_in_block++;
        }
    }

    logdebug("[%d] " PRADDRESS " seeking #%" PRIu32 " done (#%" PRIu32 ") (%" PRIu32 " bytes) (%" PRIu32 " pos-bh)",
             settings.file, address, settings.record, record, position, position - fh.size);

    return SeekValue{ address, record, position, block, timestamp, record_address, bytes_in_block, records_in_block };
}

File Storage::file(uint8_t file) {
    verify_opened();

    return File{ this, file };
}

uint32_t Storage::fsck(ProgressCallbacks *progress) {
    verify_opened();

    loginfo("fsck: begin");

    auto file0 = this->file(0);
    auto file1 = this->file(1);

    if (!file0.seek_end()) {
        logwarn("seek eof failed");
        return false;
    }

    if (!file1.seek_end()) {
        logwarn("seek eof failed");
        return false;
    }

    auto total_size = file0.size() + file1.size();
    auto tracker = ProgressTracker{ progress, Operation::Fsck, "fsck", "", total_size };

    fsck(file0, tracker);
    fsck(file1, tracker);

    tracker.finished();

    loginfo("fsck: done (%" PRIu32 "ms)", tracker.elapsed());

    return true;
}

uint32_t Storage::fsck(File &opened_file, ProgressTracker &tracker) {
    auto buffer_size = DefaultWorkerPoolSize;
    auto buffer = (uint8_t *)fk_malloc(buffer_size);
    auto remaining = opened_file.size();

    FK_ASSERT(opened_file.seek(0));

    while (remaining > 0) {
        auto to_read = std::min<int32_t>(buffer_size, remaining);
        auto nread = opened_file.read(buffer, to_read);
        FK_ASSERT(nread == to_read);
        tracker.update(nread);
        remaining -= nread;
    }

    fk_free(buffer);

    return true;
}

void Storage::verify_opened() const {
    FK_ASSERT(free_block_ != InvalidBlock);
}

void Storage::verify_mutable() const {
    FK_ASSERT(!read_only_);
}

SavedState Storage::save() const {
    SavedState state;
    state.timestamp = timestamp_;
    state.free_block = free_block_;
    state.version = version_;
    static_assert(sizeof(files_) == sizeof(state.files), "sizeof(files_) == sizeof(state.files)");
    memcpy(state.files, files_, sizeof(files_));
    return state;
}

void Storage::restore(SavedState const &state) {
    static_assert(sizeof(files_) == sizeof(state.files), "sizeof(files_) == sizeof(state.files)");
    memcpy(files_, state.files, sizeof(files_));
    timestamp_ = state.timestamp;
    free_block_ = state.free_block;
    version_ = state.version;
}

bool Storage::flush() {
    if (memory_.flush() <= 0) {
        return false;
    }

    return true;
}

}
