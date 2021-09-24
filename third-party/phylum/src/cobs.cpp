#include "cobs.h"

namespace phylum {

cobs_writer::cobs_writer(io_writer *target, write_buffer &buffer) : target_(target), buffer_(buffer) {
    assert(buffer_.size() >= 256);
}

cobs_writer::~cobs_writer() {
}

int32_t cobs_writer::write(uint8_t const *data, size_t size) {
    assert(data != nullptr);
    assert(size > 0);

    auto source_ptr = data;
    auto wrote = 0;

    while (true) {
        auto end_of_data = source_ptr == data + size;
        auto buffer_full = buffer_.position() == 0xfe;
        auto zero_byte = !end_of_data && *source_ptr == 0;

        if (end_of_data || buffer_full || zero_byte) {
            int32_t buffered = buffer_.position();
            auto bytes_wrote = buffered + 1;

            uint8_t code = buffered + 1;
            if (target_->write(&code, 1) != 1) {
                phyerrorf("cobs: writing code");
                return -1;
            }

            if (buffered > 0) {
                if (target_->write(buffer_.ptr(), buffered) != buffered) {
                    phyerrorf("cobs: writing buffer (%d)", buffered);
                    return -1;
                }

                buffer_.clear();
            }

            wrote += bytes_wrote;

            // phydebugf("end=%d full=%d zero=%d flag=%d buffered=%d wrote=%d", end_of_data, buffer_full, zero_byte, buffered, wrote);

            if (end_of_data) {
                break;
            }

            if (zero_byte) {
                if (buffer_full) {
                    uint8_t code = 1;
                    if (target_->write(&code, 1) != 1) {
                        phyerrorf("cobs: writing code");
                        return -1;
                    }
                    wrote++;
                }
                source_ptr++;
            }
        }
        else {
            buffer_.write(*source_ptr++);
        }
    }

    uint8_t end = 0;
    if (target_->write(&end, 1) != 1) {
        phyerrorf("cobs: writing end");
        return -1;
    }

    wrote++;

    if (!return_bytes_wrote_) {
        return size;
    }

    return wrote;
}

cobs_reader::cobs_reader(io_reader *target) : target_(target) {
}

int32_t cobs_reader::read(uint8_t *data, size_t size) {
    assert(data != nullptr);
    assert(size > 0);

	auto decode = data;

	for (uint8_t code = 0xff, block = 0; ; --block) {
        uint8_t byte = 0;
        if (target_->read(&byte, 1) != 1) {
            break;
        }

        if (block) {
            *decode++ = byte;
        }
        else {
            auto terminated = code != 0xff;
            block = code = byte;
            if (code == 0x00) {
                break;
            }
            if (terminated) {
                *decode++ = 0;
            }
        }
    }

	return decode - data;
}

} // namespace phylum
