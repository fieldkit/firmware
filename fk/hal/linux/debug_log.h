#pragma once

#if defined(linux)

#include <cassert>
#include <cstring>
#include <cstdlib>
#include <cstdint>

#include <iostream>
#include <vector>
#include <list>

#include "common.h"
#include "storage/storage.h"

namespace fk {

enum class OperationType {
    Opened,
    EraseBlock,
    Write,
    Read
};

class LogEntry {
private:
    OperationType type_;
    BlockAddress address_;
    uint8_t *ptr_;
    size_t size_;
    uint8_t *copy_;

public:
    LogEntry(OperationType type, BlockAddress address, uint8_t *ptr) :
        type_(type), address_(address), ptr_(ptr), size_(0), copy_(nullptr) {
    }

    LogEntry(OperationType type, BlockAddress address, uint8_t *ptr, size_t size) :
        type_(type), address_(address), ptr_(ptr), size_(size), copy_(nullptr) {
    }

    virtual ~LogEntry() {
        free_backup();
    }

public:
    OperationType type() const {
        return type_;
    }

    BlockAddress address() const {
        return address_;
    }

    void backup();

    void undo();

    void free_backup();

    bool can_undo() {
        return type_ == OperationType::Write || type_ == OperationType::EraseBlock;
    }

public:
    friend ostreamtype& operator<<(ostreamtype& os, const LogEntry& e);

};

class StorageLog {
private:
    bool copy_on_write_{ false };
    bool logging_{ false };
    std::list<LogEntry> entries_;

public:
    StorageLog() { }

public:
    void append(LogEntry &&entry);

    void undo(size_t number);

    void logging(bool logging) {
        logging_ = logging;
    }

    int32_t size() const {
        return entries_.size();
    }

    void clear() {
        entries_.erase(entries_.begin(), entries_.end());
    }

    void copy_on_write(bool enabled) {
        copy_on_write_ = enabled;
    }

    int32_t number_of(OperationType type) const {
        auto n = 0;
        for (auto &e : entries_) {
            if (e.type() == type) n++;
        }
        return n;
    }

    std::list<LogEntry> &entries() {
        return entries_;
    }

public:
    friend ostreamtype& operator<<(ostreamtype& os, const StorageLog& e);

};

}

#endif // linux
