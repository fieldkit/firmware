#include <algorithm>
#include <phylum.h>

#include "storage/storage.h"

#include "platform.h"
#include "hal/random.h"
#include "progress_tracker.h"
#include "utilities.h"

#include "storage/file_ops_darwin.h"
#include "storage/file_ops_phylum.h"

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
    BlockNumber start;
    uint32_t nblocks;

    BlockNumber middle_block() const {
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

uint32_t RecordTail::sign() {
    return phylum::crc32_checksum((uint8_t *)this, sizeof(RecordTail) - sizeof(uint32_t));
}

bool RecordTail::valid() {
    return size > 0 && size != InvalidSize && sign() == crc;
}

SeekSettings SeekSettings::end_of(uint8_t file) {
    return SeekSettings{ file, LastRecord };
}

Storage::Storage(DataMemory *memory, Pool &pool, bool read_only)
    : data_memory_(memory), pool_(&pool), memory_(memory, pool), statistics_data_memory_(data_memory_), bad_blocks_(memory, pool),
      phylum_{ &statistics_data_memory_ }, read_only_(read_only) {
    FK_ASSERT(memory != nullptr);
    files_ = pool.malloc<FileHeader>(NumberOfFiles);
}

Storage::~Storage() {
    if (!memory_.flush()) {
        logerror("flush failed");
    }
}

bool Storage::begin() {
    if (phylum_.mount()) {
        data_ops_ = new (pool_) phylum_ops::DataOps(*this);
        meta_ops_ = new (pool_) phylum_ops::MetaOps(*this);
        loginfo("storage-begin: phylum");
        return true;
    }

    if (begin_internal()) {
        data_ops_ = new (pool_) darwin::DataOps(*this);
        meta_ops_ = new (pool_) darwin::MetaOps(*this);
        loginfo("storage-begin: darwin");
        return true;
    }

    loginfo("storage-begin: nothing");
    return false;
}

bool Storage::clear() {
    loginfo("storage: clearing");

#if !defined(__SAMD51__)
    if (!clear_internal()) {
        return false;
    }

    data_ops_ = new (pool_) darwin::DataOps(*this);
    meta_ops_ = new (pool_) darwin::MetaOps(*this);

    return true;
#endif

    for (auto block = 0u; block < data_memory_->geometry().nblocks; ++block) {
        auto block_size = data_memory_->geometry().block_size;
        auto address = block * block_size;
        if (data_memory_->erase(address, block_size) < 0) {
            logerror("erasing block=%" PRIu32, block);
        }
    }

    if (!phylum_.format()) {
        logerror("format");
        return false;
    }

    auto data_ops = new (pool_) phylum_ops::DataOps(*this);

    if (!data_ops->touch(*pool_)) {
        logerror("touch");
        return false;
    }

    if (!phylum_.sync()) {
        logerror("sync");
        return false;
    }

    loginfo("storage: cleared");

    data_ops_ = data_ops;
    meta_ops_ = new (pool_) phylum_ops::MetaOps(*this);

    return true;
}

DataOps *Storage::data_ops() {
    return data_ops_;
}

MetaOps *Storage::meta_ops() {
    return meta_ops_;
}

int32_t Storage::installed() {
    return data_memory_->geometry().total_size;
}

int32_t Storage::used() {
    return 0;
}

FileReader *Storage::file_reader(FileNumber file_number, Pool &pool) {
    return new (pool) darwin::FileReader{ *this, file_number, pool };
}

bool Storage::valid_block_header(BlockHeader &header) const {
    return header.valid() && (version_ == InvalidVersion || header.version == version_);
}

Storage::BlocksAfter Storage::find_blocks_after(uint32_t starting, FileNumber file, RecordNumber record) {
    logdebug("[-] block: blk %" PRIu32 " sequential scan", starting);

    auto g = memory_.geometry();
    auto testing = starting;
    auto free = starting;
    auto tail = starting;
    auto bad_blocks = 0u;
    auto records_skipped = 0u;
    auto bytes_skipped = 0u;

    while (true) {
        auto head_address = (uint32_t)(g.block_size * testing);
        auto tail_address = head_address + g.block_size - SizeofBlockTail;

        // This is mostly to check for reaching the end of memory. TODO Wrap around.
        if (!g.is_address_valid(head_address) || !g.is_address_valid(tail_address)) {
            return BlocksAfter{ };
        }

        BlockHeader block_header;
        if (memory_.read(head_address, (uint8_t *)&block_header, sizeof(block_header)) != sizeof(block_header)) {
            return BlocksAfter{ };
        }

        if (!valid_block_header(block_header)) {
            if (bad_blocks == 0) {
                logdebug("[-] block: blk %" PRIu32 " invalid header (first)", testing);
                free = testing;
            }
            else if (bad_blocks == 10) {
                logdebug("[-] block: blk %" PRIu32 " invalid header (fail)", testing);
                break;
            }
            else {
                logdebug("[-] block: blk %" PRIu32 " invalid header (%d)", testing, bad_blocks);
            }
            testing++;
            bad_blocks++;
            continue;
        }
        else {
            if (file == InvalidFile || file == block_header.file) {
                tail = testing;
                free = testing + 1;
            }

            bad_blocks = 0;
        }

        if (record != InvalidRecord) {
            if (tail != starting) {
                break;
            }
        }

        BlockTail block_tail;
        if (memory_.read(tail_address, (uint8_t *)&block_tail, sizeof(block_tail)) != sizeof(block_tail)) {
            return BlocksAfter{ };
        }

        if (!block_tail.verify_hash()) {
            logdebug("[-] block: blk %" PRIu32 " invalid tail", testing);
            testing++;
            bad_blocks++;
            continue;
        }

        if (!is_address_valid(block_tail.linked)) {
            logdebug("[-] block: blk %" PRIu32 " invalid linked address (" PRADDRESS ")", testing, block_tail.linked);
            testing++;
            bad_blocks++;
            continue;
        }

        auto linked = block_tail.linked / g.block_size;

        logdebug("[-] block: blk %" PRIu32 " -> blk %" PRIu32 " %" PRIu32, testing, linked, block_tail.bytes_in_block);

        bytes_skipped += block_tail.bytes_in_block;
        records_skipped += block_tail.records_in_block;

        FK_ASSERT(testing != linked);

        testing = linked;
    }

    logdebug("[-] block: scan done free = blk %" PRIu32 " tail = blk %" PRIu32 " %u bytes skipped", free, tail, bytes_skipped);

    return BlocksAfter{ starting, free, tail, bytes_skipped, records_skipped };
}

bool Storage::begin_internal() {
    auto g = memory_.geometry();
    auto started = fk_uptime();

    free_block_ = 0;
    timestamp_ = 0;

    file_headers_block_ = 0;
    for (auto i = 0u; i < NumberOfFiles; ++i) {
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
        auto block = range.middle_block();
        auto address = block * g.block_size;
        if (memory_.read(address, (uint8_t *)&block_header, sizeof(block_header)) != sizeof(block_header)) {
            logerror("[-] read failed " PRADDRESS, address);
            return false;
        }

        if (valid_block_header(block_header)) {
            logtrace("[%d] valid block (blk %" PRIu32 ") (" PRADDRESS ") (v = %" PRIu32 ") (ts = %" PRIu32 ")",
                     block_header.file, block, address, block_header.version, block_header.timestamp);

            // This is open specific
            FK_ASSERT(block_header.verify_hash());

            file_headers_block_ = block;
            for (auto i = 0u; i < NumberOfFiles; ++i) {
                files_[i] = block_header.files[i];
            }

            timestamp_ = block_header.timestamp;
            version_ = block_header.version;
            range = range.second_half();
            had_valid_blocks = true;
            // End of open specific
        }
        else {
            logtrace("[-] invalid block (blk %" PRIu32 ") (" PRADDRESS ")", block, address);
            range = range.first_half();
        }
    }

    if (!had_valid_blocks) {
        return false;
    }

    auto blocks_after = find_blocks_after(range.start, InvalidFile, InvalidRecord);
    if (!blocks_after) {
        return false;
    }

    free_block_ = blocks_after.free;

    logdebug("[-] block: blk %" PRIu32 " found end (%" PRIu32 "ms)", free_block_, fk_uptime() - started);

    for (auto i = 0u; i < NumberOfFiles; ++i) {
        logdebug("[%d] header tail (" PRADDRESS ") (R-%" PRIu32 ")", i, files_[i].tail, files_[i].record);
    }

    return true;
}

bool Storage::clear_internal() {
    auto g = memory_.geometry();

    free_block_ = 0;
    timestamp_ = 0;
    version_ = fk_random_i32(0, INT32_MAX);

    for (auto i = 0u; i < NumberOfFiles; ++i) {
        files_[i] = { };
    }

    if (!data_memory_->begin()) {
        return false;
    }

    auto range = BlockRange{ 0, g.nblocks };
    while (!range.empty()) {
        uint32_t address = range.middle_block() * g.block_size;
        logdebug("[" PRADDRESS "] erasing block", address);
        auto err = memory_.erase(address, g.block_size);
        if (err < 0) {
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

    // Find a good block.
    for (auto i = 0; i < StorageAvailableBlockLookAhead; ++i) {
        auto block = free_block_;
        auto address = block * g.block_size;

        free_block_++;

        auto bad = bad_blocks_.is_address_bad(address);
        if (bad) {
            logerror("[%d] allocating ignoring bad block: blk %" PRIu32 " (marked bad)", file, block);
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
            logerror("[%d] allocating ignoring bad block: blk %" PRIu32 " (factory bad)", file, block);
            bad_blocks_.mark_address_as_bad(address);
            continue;
        }

        // Erase new block and write header.
        rv = memory_.erase(address, g.block_size);
        if (rv < 0) {
            logerror("[%d] allocating ignoring bad block: blk %" PRIu32 " (erase failed)", file, block);
            bad_blocks_.mark_address_as_bad(address);
            continue;
        }

        loginfo("[%d] allocating block blk %" PRIu32 " ts=%" PRIu32 " (" PRADDRESS ") (pta=" PRADDRESS ") (try #%d) (R-%" PRIu32 ") (%" PRIu32 " bytes)",
                file, block, timestamp_, address, previous_tail_address, i, files_[file].record, files_[file].size);

        // First sector is the block header. We force write this to
        // ensure the block is good.
        auto first_record_address = address + g.sector_size();

        // Do this before writing header so the new value gets written.
        file_headers_block_ = address / g.block_size;
        files_[file].tail = first_record_address;

        BlockHeader block_header;
        bzero((void *)&block_header, sizeof(BlockHeader));
        block_header.magic.fill();
        block_header.file = file;
        block_header.timestamp = timestamp_;
        block_header.version = version_;
        for (auto i = 0u; i < NumberOfFiles; ++i) {
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
            FK_ASSERT(address != 0);
            FK_ASSERT(address > previous_tail_address);

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

    logtrace("[%d] seeking R-%" PRIu32, settings.file, settings.record);

    // Binary search for the block to start with.
    BlockHeader file_block_header;
    file_block_header.timestamp = 0;
    auto range = BlockRange{ 0, g.nblocks };
    while (!range.empty()) {
        BlockHeader block_header;
        auto address = range.middle_block() * g.block_size;
        auto rv = memory_.read(address, (uint8_t *)&block_header, sizeof(block_header));
        if (rv != sizeof(block_header)) {
            logerror("[%d] read failed " PRADDRESS, settings.file, address);
            return SeekValue{ };
        }

        if (valid_block_header(block_header)) {
            auto &bfh = block_header.files[settings.file];
            logtrace("[%d] valid block (%d) (blk %" PRIu32 ") (" PRADDRESS ") (v = %" PRIu32 ") (ts = %" PRIu32 ") (%" PRIu32 " bytes bob) (" PRADDRESS ")",
                     block_header.file, settings.file, address / g.block_size, address, block_header.version, block_header.timestamp, bfh.size, bfh.tail);

            // This is seek specific
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
            // End of seek specific
        }
        else {
            logtrace("[-] invalid block (blk %" PRIu32 ") (" PRADDRESS ")", address / g.block_size, address);
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
    auto records_visited = 0u;
    auto blocks_visited = 0u;

    // If the address is invalid then this file is empty, nothing to be found,
    // this is success if they were looking for the last record of the file.
    if (!is_address_valid(address)) {
        return SeekValue{ };
    }

    logdebug("[%d] " PRADDRESS " seeking R-%" PRIu32 " (position = %" PRIu32 ") block scan done, from blk %" PRIu32 " (bsz = %" PRIu32 " bytes)",
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
                /**
                 * This is here so that we can get the correct record_address. This
                 * will typically happen when the file block header pointed directly
                 * to the tail of the file, so we have the record number but
                 * record_address is unset because we haven't scanned the block yet.
                 */
                if (record_address == 0 && record > 0) {
                    auto previous_address = address;
                    address = g.start_of_block(previous_address);
                    logdebug("[%d] " PRADDRESS " rescanning block from " PRADDRESS "", settings.file, previous_address, address);

                    if (g.is_start_of_block_or_header(previous_address, SizeofBlockHeader)) {
                        return SeekValue{ };
                    }
                    continue;
                }

                /**
                 * Files headers written to blocks contain the last
                 * address for that file when that block was
                 * allocated. Because blocks are allocated
                 * sequentially, we know that any new blocks added to
                 * that file have to come after the block the header
                 * was read from.
                 *
                 * So, we don't need to look for more blocks after
                 * this block if the latest file header from some
                 * future block has this tail address. Any future
                 * blocks have to have been allocated after that
                 * block with the header.
                 *
                 * It's important that the file headers come from a
                 * block after this one. It's ok if the block address
                 * is earlier than our address, though because we
                 * could still be filling in the block.
                 */
                auto block = address / g.block_size;
                if (file_headers_block_ > block) {
                    if (files_[settings.file].tail <= address) {
                        logdebug("[%d] " PRADDRESS " found real tail", settings.file, address);
                        break;
                    }
                }

                logdebug("[%d] fhb = blk %" PRIu32 " blk %" PRIu32 " " PRADDRESS " " PRADDRESS, settings.file,
                         file_headers_block_, block, files_[settings.file].tail, address);

                /**
                 * Now, we can't be sure we're at the end of the
                 * file so we need to scan past this block. What may
                 * have happened is there some bad blocks after this
                 * block and the file resumes after.
                 */
                logdebug("[%d] " PRADDRESS " end of block records (blk %" PRIu32 ") finding block after", settings.file, address, block);

                auto blocks_after = find_blocks_after(block, settings.file, settings.record);
                if (!blocks_after) {
                    logdebug("[%d] " PRADDRESS " error finding blocks after (blk %" PRIu32 ")", settings.file, address, block);
                    return SeekValue{};
                }

                /**
                 * WhenABadBlockStumblesBinarySearchAndSeekNeedsToTraverseTailBlocksLinearlyTheSizeAndPositionWouldBeWrong
                 * The idea is here that find_blocks_after may have
                 * actually done just that, and traversed across some
                 * blocks at the end of the file to arrive where we
                 * are now. This will update our position to account
                 * for the file that it skipped over. We subtract how
                 * much we've already seen in our current block
                 * because the find_blocks_after will just look at
                 * block tails, even for the block it's beginning in.
                 * The if is to keep us from doing this if we haven't
                 * actually traversed the block yet.
                 */
                if (blocks_after.bytes > 0) {
                    position += blocks_after.bytes - bytes_in_block;
                    records_visited += blocks_after.records - records_in_block;
                }

                if (blocks_after.tail == block) {
                    logdebug("[%d] " PRADDRESS " found real tail (blk %" PRIu32 ") done", settings.file, address, block);
                    break;
                }

                blocks_visited++;
                block = blocks_after.tail;
                address = block * g.block_size;
                record_address = address;
                record_address = 0u;
                bytes_in_block = 0u;
                records_in_block = 0u;

                logdebug("[%d] " PRADDRESS " resuming head (blk %" PRIu32 ")", settings.file, address, block);

                continue;
            }
            else {
                address = partial_aligned;
            }
        }

        // We've got a valid record header so let's remember this position.
        record_address = address;
        records_visited++;

        // Is this the record they're looking for, or is this after the record
        // they're looking for?
        if (settings.record != InvalidRecord) {
            if (record_head.record == settings.record) {
                logverbose("[%d] " PRADDRESS " found record R-%" PRIu32, settings.file, address, settings.record);
                record = record_head.record;
                break;
            }
            if (settings.record < record_head.record) {
                logverbose("[%d] " PRADDRESS " found nearby record R-%" PRIu32, settings.file, address, settings.record);
                break;
            }
        }

        // Keep track of maximum record for the file.
        if (record_head.record >= record) {
            record = record_head.record + 1;
        }

        auto record_length = (uint32_t)(sizeof(RecordHeader) + record_head.size + sizeof(RecordTail));

        logverbose("[%d] " PRADDRESS " seeking %4" PRIu32 "/%4" PRIu32 " (R-%" PRIu32 ")",
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

    logdebug("[%d] " PRADDRESS " seeking R-%" PRIu32 " done (R-%" PRIu32 ") (%" PRIu32 " bytes) (%" PRIu32 " pos-bh) (%u/%u visited)",
             settings.file, address, settings.record, record, position, position - fh.size, records_visited, blocks_visited);

    return SeekValue{ address, record, position, block, timestamp, record_address, bytes_in_block, records_in_block };
}

File Storage::file(uint8_t file_number) {
    return file(file_number, *pool_);
}

File Storage::file(uint8_t file_number, Pool &pool) {
    verify_opened();

    return File{ this, file_number, pool };
}

uint32_t Storage::fsck(ProgressCallbacks *progress) {
    verify_opened();

    loginfo("fsck: begin");

    auto file0 = this->file(0);
    auto file1 = this->file(1);

    loginfo("fsck: seeding end of 0");
    if (!file0.seek_end()) {
        logwarn("seek eof failed");
        return false;
    }

    loginfo("fsck: seeding end of 1");
    if (!file1.seek_end()) {
        logwarn("seek eof failed");
        return false;
    }

    loginfo("fsck: size of 0 = %" PRIu32 " size of 1 = %" PRIu32, file0.size(), file1.size());

    auto total_size = file0.size() + file1.size();
    auto tracker = ProgressTracker{ progress, Operation::Fsck, "fsck", "", total_size };

    loginfo("fsck: walking 0");
    fsck(file0, tracker);

    loginfo("fsck: walking 1");
    fsck(file1, tracker);

    tracker.finished();

    loginfo("fsck: done (%" PRIu32 "ms)", tracker.elapsed());

    return true;
}

uint32_t Storage::fsck(File &opened_file, ProgressTracker &tracker) {
    auto buffer_size = StandardPageSize;
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
    static_assert(sizeof(state.files) == sizeof(FileHeader) * NumberOfFiles, "sizeof(state.files) == sizeof(FileHeader) * NumberOfFiles");
    memcpy(state.files, files_, sizeof(state.files));
    return state;
}

void Storage::restore(SavedState const &state) {
    static_assert(sizeof(state.files) == sizeof(FileHeader) * NumberOfFiles, "sizeof(state.files) == sizeof(FileHeader) * NumberOfFiles");
    memcpy(files_, state.files, sizeof(state.files));
    timestamp_ = state.timestamp;
    free_block_ = state.free_block;
    version_ = state.version;
}

bool Storage::flush() {
    if (memory_.flush() <= 0) {
        return false;
    }

    statistics_data_memory_.log_statistics("flash usage: ");

    return true;
}

}
