#include <tiny_printf.h>

#include "writer.h"

namespace fk {

static void write_buffered_writer(char c, void *arg) {
    reinterpret_cast<BufferedWriter*>(arg)->write(c);
}

BufferedWriter::BufferedWriter(Writable *writer) : writer_(writer) {
}

BufferedWriter::~BufferedWriter() {
    flush();
}

int32_t BufferedWriter::write(const char *s, ...) {
    va_list args;
    va_start(args, s);
    auto r = tiny_vfctprintf(write_buffered_writer, this, s, args);
    va_end(args);
    return r;
}

int32_t BufferedWriter::write(char c) {
    if (c != 0) {
        buffer_[position_++] = c;
        if (position_ == buffer_size_ - 1) {
            return flush();
        }
    }
    return 1;
}

int32_t BufferedWriter::flush() {
    if (position_ > 0) {
        buffer_[position_] = 0;
        auto rv = writer_->write(buffer_, position_);
        position_ = 0;
        return rv;
    }
    return position_;
}

}
