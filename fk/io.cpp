#include <tiny_printf.h>
#include <algorithm>

#include "io.h"

namespace fk {

static void write_buffered_writer(char c, void *arg) {
    if (c != 0) {
        reinterpret_cast<BufferedWriter *>(arg)->write(c);
    }
}

BufferedWriter::BufferedWriter(Writer *writer, uint8_t *buffer, size_t size) : writer_(writer), buffer_(buffer), buffer_size_(size) {
}

BufferedWriter::~BufferedWriter() {
    flush();
}

int32_t BufferedWriter::write(uint8_t const *buffer, size_t size) {
    FK_ASSERT(buffer_size_ > 0);

    size_t wrote = 0;

    // NOTE We could avoid copying in here if the data being written is larger.

    while (wrote < size) {
        auto available = buffer_size_ - position_;
        if (available == 0) {
            return -1;
        }
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
    buffer_[position_++] = c;
    if (position_ == buffer_size_ - 1) {
        return flush();
    }
    return 1;
}

int32_t BufferedWriter::flush() {
    if (writer_ == nullptr) {
        return -1;
    }
    if (position_ > 0) {
        // This assertion is probably obsolete. I need to cleanup the
        // PoolPointer work for NetworkConnections. At one point the cpool was
        // freeing connections while they were still in use and we'd see crashes
        // in here.
        FK_ASSERT(writer_ != nullptr);
        auto rv = writer_->write(buffer_, position_);
        position_ = 0;
        return rv;
    }
    return position_;
}

BufferedReader::BufferedReader(Reader *reader, uint8_t *buffer, size_t buffer_size, size_t bytes_read)
    : reader_(reader), buffer_(buffer), buffer_size_(buffer_size), bytes_read_(bytes_read) {
}

BufferedReader::~BufferedReader() {
}

int32_t BufferedReader::read(uint8_t *buffer, size_t size) {
    auto returning = 0u;
    while (returning < size) {
        auto available = bytes_read_ - position_;
        if (bytes_read_ == 0 || available == 0) {
            if (reader_ == nullptr) {
                return -1;
            }

            auto nread = reader_->read(buffer_, buffer_size_);
            if (nread <= 0) {
                return 0;
            }

            position_ = 0;
            bytes_read_ = nread;
        }

        auto reading = std::min<size_t>(bytes_read_ - position_, size - returning);
        memcpy(buffer + returning, buffer_ + position_, reading);
        position_ += reading;
        returning += reading;
    }

    return returning;
}

int32_t BufferedReader::skip(size_t bytes) {
    position_ += bytes;
    return position_ < bytes_read_ ? bytes : -1;
}

BufferedReader BufferedReader::beginning() const {
    return BufferedReader{ nullptr, buffer_, buffer_size_, bytes_read_ };
}

BufferedReader BufferedReader::remaining() const {
    BufferedReader reader{ nullptr, buffer_, buffer_size_, bytes_read_ };
    reader.skip(position_);
    return reader;
}

static bool write_callback(pb_ostream_t *stream, const uint8_t *buf, size_t c) {
    auto s = reinterpret_cast<Writer *>(stream->state);
    return s->write(buf, c) == (int32_t)c;
}

static bool read_callback(pb_istream_t *stream, uint8_t *buf, size_t c) {
    auto s = reinterpret_cast<Reader *>(stream->state);
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

pb_istream_t pb_istream_from_readable(Reader *s, size_t bytes_left) {
    pb_istream_t stream = { &read_callback, (void *)s, bytes_left };
    return stream;
}

Buffer::Buffer(uint8_t *ptr, size_t size) : ptr_(ptr), size_(size), position_(0) {
}

void Buffer::write(char c) {
    FK_ASSERT(position_ < size_);
    ptr_[position_++] = c;
}

bool Buffer::full() const {
    return position_ == size_;
}

void Buffer::clear() {
    position_ = 0;
}

} // namespace fk
