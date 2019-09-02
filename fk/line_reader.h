#pragma once

#include "writer.h"
#include "platform.h"

namespace fk {

template<size_t N>
class LineReader {
private:
    Reader *reader_;
    char buffer_[N];
    size_t buffer_size_{ N };
    size_t position_{ 0 };
    size_t cursor_{ 0 };
    size_t line_{ 0 };

public:
    LineReader(Reader *reader) : reader_(reader) {
    }

public:
    bool read_line_sync(const char **line, uint32_t to) {
        auto deadline = fk_uptime() + to;

        while (fk_uptime() < deadline) {
            if (read_line_async(line)) {
                return true;
            }

            fk_delay(10);
        }

        return false;
    }

    bool read_line_async(const char **line) {
        (*line) = nullptr;

        auto available = (size_t)(buffer_size_ - position_);
        if (available > 0) {
            auto nread = reader_->read((uint8_t *)(buffer_ + position_), available);
            if (nread < 0) {
                return nread;
            }
            if (nread > 0) {
                position_ += nread;
            }
        }

        for (size_t i = cursor_; i < position_; ++i) {
            auto c = buffer_[i];
            if (c == '\n' || c == '\r') {
                buffer_[i] = 0;
                *line = buffer_ + line_;
                cursor_ = i + 1;
                line_ = i + 1;
                if (cursor_ == position_) {
                    cursor_ = 0;
                    position_ = 0;
                    line_ = 0;
                }
                if (strlen(*line) > 0) {
                    return true;
                }
            }
        }

        return false;
    }

};

}
