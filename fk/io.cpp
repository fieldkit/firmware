#include <tiny_printf.h>
#include <algorithm>

#include "io.h"

namespace fk {

static void write_buffered_writer(char c, void *arg) {
    reinterpret_cast<BufferedWriter*>(arg)->write(c);
}

BufferedWriter::BufferedWriter(Writer *writer) : writer_(writer) {
}

BufferedWriter::~BufferedWriter() {
    flush();
}

int32_t BufferedWriter::write(uint8_t const *buffer, size_t size) {
    FK_ASSERT(buffer_size_ > 0);

    size_t wrote = 0;

    while (wrote < size) {
        auto available = buffer_size_ - position_;
        auto writing = std::min<size_t>(available, size - wrote);
        memcpy(buffer_ + position_, buffer + wrote, writing);
        wrote += writing;
        position_ += writing;
        if (position_ == buffer_size_) {
            flush();
        }
    }

    return wrote;
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
        auto rv = writer_->write(buffer_, position_);
        position_ = 0;
        return rv;
    }
    return position_;
}

Base64Reader::Base64Reader(Reader *target) : target_(target) {
}

static size_t hexchr2bin(uint8_t const hex, uint8_t *p) {
    if (hex >= '0' && hex <= '9') {
        *p = hex - '0';
    }
    else if (hex >= 'A' && hex <= 'F') {
        *p = hex - 'A' + 10;
    }
    else if (hex >= 'a' && hex <= 'f') {
        *p = hex - 'a' + 10;
    }
    else {
        return 0;
    }
    return 1;
}

int32_t Base64Reader::read(uint8_t *buffer, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        uint8_t encoded[2];
        if (target_->read(encoded, sizeof(encoded)) != 2) {
            return 0;
        }
        uint8_t decoded[2];
        if (!hexchr2bin(encoded[0], &decoded[0]) || !hexchr2bin(encoded[1], &decoded[1])) {
            return 0;
        }
        buffer[i] = (decoded[0] << 4) | decoded[1];
    }
    return size;
}

Base64Writer::Base64Writer(Writer *target) : target_(target) {
}

constexpr char Hex[] = "0123456789ABCDEF";

int32_t Base64Writer::write(uint8_t const *buffer, size_t size) {
    for (size_t i = 0; i < size; i++) {
        char encoded[2];
        encoded[0] = Hex[buffer[i] >> 4];
        encoded[1] = Hex[buffer[i] & 0x0F];
        if (target_->write((uint8_t const *)encoded, sizeof(encoded)) != sizeof(encoded)) {
            return i;
        }
    }
    return size;
}

static bool write_callback(pb_ostream_t *stream, const uint8_t *buf, size_t c) {
    auto s = reinterpret_cast<Writer*>(stream->state);
    return s->write(buf, c) == (int32_t)c;
}

static bool read_callback(pb_istream_t *stream, uint8_t *buf, size_t c) {
    auto s = reinterpret_cast<Reader*>(stream->state);
    auto nread = s->read(buf, c);
    if (nread <= 0) {
        stream->bytes_left = 0; /* EOF */
    }
    return nread == (int32_t)c;
}

pb_ostream_t pb_ostream_from_writable(Writer *s) {
    pb_ostream_t stream = { &write_callback, (void *)s, SIZE_MAX, 0 };
    return stream;
}

pb_istream_t pb_istream_from_readable(Reader *s) {
    pb_istream_t stream = { &read_callback, (void *)s, SIZE_MAX };
    return stream;
}

}
