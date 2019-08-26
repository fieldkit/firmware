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

static bool write_callback(pb_ostream_t *stream, const uint8_t *buf, size_t c) {
    auto s = reinterpret_cast<Writable*>(stream->state);
    return s->write(buf, c) == (int32_t)c;
}

static bool read_callback(pb_istream_t *stream, uint8_t *buf, size_t c) {
    auto s = reinterpret_cast<Readable*>(stream->state);
    auto nread = s->read(buf, c);
    if (nread <= 0) {
        stream->bytes_left = 0; /* EOF */
    }
    return nread == (int32_t)c;
}

pb_ostream_t pb_ostream_from_writable(Writable *s) {
    pb_ostream_t stream = { &write_callback, (void *)s, SIZE_MAX, 0 };
    return stream;
}

pb_istream_t pb_istream_from_readable(Readable *s) {
    pb_istream_t stream = { &read_callback, (void *)s, SIZE_MAX };
    return stream;
}

}
