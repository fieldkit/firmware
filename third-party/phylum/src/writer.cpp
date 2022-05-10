#include "writer.h"

namespace phylum {

int32_t noop_writer::write(uint8_t const * /*data*/, size_t size) {
    if (remaining_ == UINT32_MAX) {
        return size;
    }

    auto copying = std::min<size_t>(remaining_, size);
    remaining_ -= copying;
    return copying;
}

int32_t varint_decoder::write(uint8_t const *data, size_t size) {
    auto nread = 0;

    if (done_) {
        return 0;
    }

    read_buffer buffer{ data, size };
    while (true) {
        uint8_t byte = 0;
        auto err = buffer.read_byte(&byte);
        if (err < 0) {
            break;
        }
        if (err != 1) {
            return -1;
        }

        nread++;
        bytes_read_++;

        uint32_t ll = byte;
        value_ += ((ll & 0x7F) << width_);
        width_ += 7;

        if (!(byte & 0x80)) {
            done_ = true;
            break;
        }
    }

    return nread;
}

blake2b_writer::blake2b_writer(io_writer *target) : target_(target) {
    b2b_.reset(HashSize);
}

int32_t blake2b_writer::write(uint8_t const *data, size_t size) {
    auto err = target_->write(data, size);
    if (err <= 0) {
        return err;
    }
    b2b_.update(data, size);
    return err;
}

void blake2b_writer::finalize(void *hash, size_t buffer_length) {
    b2b_.finalize(hash, buffer_length);
}

} // namespace phylum
