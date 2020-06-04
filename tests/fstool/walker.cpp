#include "walker.h"

namespace fk {

FK_DECLARE_LOGGER("tests");

class BlockNavigation {
private:
    DataMemory *data_memory_;
    SequentialWrapper<BufferedPageMemory> memory_;
    Pool *pool_;

    StorageAddress block_size_{ 128 * 1024 };
    BlockNumber nblocks_{ 2048 };
    BlockNumber block_{ InvalidBlock };
    BlockHeader header_;
    BlockTail tail_;

    friend class RecordNavigation;

public:
    BlockNavigation(DataMemory *data_memory, Pool *pool) : data_memory_(data_memory), memory_(data_memory, *pool), pool_(pool) {
    }

public:
    bool move_beginning() {
        block_ = 0;
        return read();
    }

    bool move_end() {
        block_ = nblocks_; // Number of blocks.
        return read();
    }

    bool move_tail() {
        // TODO Search, read called in loop.
        return true;
    }

    bool forward() {
        if (block_ + 1 >= nblocks_) {
            return false;
        }

        block_++;
        return read();
    }

    bool backward() {
        if (block_ < 1) {
            return false;
        }

        block_--;
        return read();
    }

    BlockNumber number() const {
        return block_;
    }

    StorageAddress address() const {
        return block_ * block_size_;
    }

    BlockHeader const &header() const {
        return header_;
    }

    BlockTail const &tail() const {
        return tail_;
    }

private:
    bool read() {
        auto head_address = address();
        if (memory_.read(head_address, (uint8_t *)&header_, sizeof(header_)) != sizeof(header_)) {
            return false;
        }

        auto tail_address = head_address + block_size_ - SizeofBlockTail;
        if (memory_.read(tail_address, (uint8_t *)&tail_, sizeof(tail_)) != sizeof(tail_)) {
            return false;
        }

        return true;
    }

};

class RecordNavigation {
private:
    Pool *pool_;
    DataMemory *data_memory_;
    StorageAddress block_size_{ 128 * 1024 };
    BlockNumber nblocks_{ 2048 };
    // Mutates
    BlockNavigation *blocks_;
    RecordHeader header_;
    RecordTail tail_;
    RecordNumber number_;
    StorageAddress address_;

public:
    RecordNavigation(BlockNavigation &blocks) : pool_(blocks.pool_), data_memory_(blocks.data_memory_),
                                                block_size_(blocks.block_size_), nblocks_(blocks.nblocks_),
                                                blocks_(&blocks),
                                                address_(blocks_->address()) {
    }

public:
    bool move_beginning() {
        address_ = blocks_->address();
        return read_header();
    }

    bool forward() {
        if (address_ == blocks_->address()) {
            address_ += SizeofBlockHeader;
        }
        else {
            address_ += sizeof(RecordHeader);
            address_ += header_.size;
            address_ += sizeof(RecordTail);
        }
        return read_header();
    }

    bool backward() {
        return read_header();
    }

    RecordNumber number() const {
        return header_.record;
    }

    RecordSize size() const {
        return header_.size;
    }

    StorageAddress address() const {
        return address_;
    }

    RecordHeader const &header() const {
        return header_;
    }

    RecordTail const &tail() const {
        return tail_;
    }

private:
    static constexpr uint32_t SectorSize = 512;

    static StorageAddress partial_write_boundary_after(StorageAddress address) {
        auto padding = address % SectorSize;
        return padding == 0 ? address : address + (SectorSize - padding);
    }

    bool read_header(bool first = true) {
        if (blocks_->memory_.read(address(), (uint8_t *)&header_, sizeof(header_)) != sizeof(header_)) {
            return false;
        }

        if (header_.valid()) {
            return true;
        }

        if (!first) {
            return false;
        }

        address_ = partial_write_boundary_after(address_);

        return read_header(false);
    }

};

StorageWalker::StorageWalker(DataMemory *data_memory, Pool &pool) : data_memory_(data_memory), memory_(data_memory, pool), pool_(&pool) {
}

void StorageWalker::walk() {
    BlockNavigation blocks{ data_memory_, pool_ };

    auto counter = 100;

    while (blocks.forward()) {
        auto block = blocks.number();
        auto address = blocks.address();
        auto header = blocks.header();
        auto tail = blocks.tail();

        if (header.verify_hash()) {
            loginfo("[%4d] [" PRADDRESS "] head [%d] timestamp=%" PRIu32 " version=%" PRIu32 "", block, address, header.file, header.timestamp, header.version);
        }
        else {
            loginfo("[%4d] [" PRADDRESS "] head invalid", block, address);
        }

        if (true) {
            RecordNavigation records{ blocks };

            while (records.forward()) {
                loginfo("[%4d] [" PRADDRESS "] record R-%" PRIu32 " %" PRIu32 " bytes", block, records.address(),
                        records.number(),
                        records.size());
            }
        }

        if (tail.verify_hash()) {
            loginfo("[%4d] [" PRADDRESS "] tail %" PRIu32 " bytes %" PRIu32 " records tail=" PRADDRESS " linked=" PRADDRESS, block, address, tail.bytes_in_block, tail.records_in_block, tail.block_tail, tail.linked);
        }
        else {
            loginfo("[%4d] [" PRADDRESS "] tail invalid", block, address);
        }

        if (counter-- == 0) {
            break;
        }
    }
}

}
