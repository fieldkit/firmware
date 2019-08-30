#include "debug_log.h"

#if defined(linux)

using namespace alogging;

namespace fk {

void LogEntry::backup() {
    assert(copy_ == nullptr);

    if (can_undo()) {
        if (ptr_ != nullptr) {
            copy_ = (uint8_t *)fk_malloc(size_);
            memcpy(copy_, ptr_, size_);
        }
    }
}

void LogEntry::undo() {
    assert(copy_ != nullptr);

    if (ptr_ != nullptr) {
        memcpy(ptr_, copy_, size_);
    }
}

void LogEntry::free_backup() {
    if (copy_ != nullptr) {
        fk_free(copy_);
        copy_ = nullptr;
    }
}

void StorageLog::append(LogEntry &&entry) {
    if (!copy_on_write_ && entries_.size() > 0) {
        entries_.back().free_backup();
    }

    entries_.emplace_back(std::move(entry));

    entries_.back().backup();

    if (logging_) {
        sdebug() << "Entry: " << entry << endl;
    }
}

void StorageLog::undo(size_t number) {
    entries_.back().undo();
}

ostreamtype& operator<<(ostreamtype& os, const LogEntry& e) {
    switch (e.type_) {
    case OperationType::Opened:
        os << "Opened()";
        break;
    case OperationType::EraseBlock:
        os << "EraseBlock(" << e.address_ << ")";
        break;
    case OperationType::Read:
        os << "Read(" << e.address_ << " " << e.size_ << ")";
        break;
    case OperationType::Write:
        os << "Write(" << e.address_ << " " << e.size_ << ")";
        break;
    default:
        os << "Unknown";
        break;
    }
    return os;
}

ostreamtype& operator<<(ostreamtype& os, const StorageLog& l) {
    for (auto &e : l.entries_) {
        os << e << endl;
    }
    return os;
}

}

#endif // linux
