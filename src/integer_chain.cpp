#include "integer_chain.h"

namespace phylum {

int32_t integer_chain::seek_end_of_buffer(page_lock &/*page_lock*/) {
    auto err = db().seek_once();
    if (err < 0) {
        return err;
    }

    appendable(true);

    return 0;
}

int32_t integer_chain::write_header(page_lock &page_lock) {
    logged_task lt{ "ic-write-hdr", name() };

    assert_valid();

    db().emplace<data_chain_header_t>();

    db().terminate();

    page_lock.dirty();
    appendable(true);

    return 0;
}

int32_t integer_chain::write(uint32_t const *values, size_t length) {
    logged_task lt{ "ic-write", name() };

    auto index = 0u;
    return write_chain([&](write_buffer buffer, bool &grow) {
        int32_t written = 0;
        while (index < length) {
            int32_t needed = varint_encoding_length(values[index]);
            if ((int32_t)buffer.available() < needed) {
                grow = true;
                return written;
            }

            varint_encode(values[index], buffer.cursor(), needed);
            index++;
            buffer.skip(needed);

            return needed;
        }
        return written;
    });
}

int32_t integer_chain::read(uint32_t *values, size_t length) {
    logged_task lt{ "ic-read", name() };

    int32_t index = 0;
    return read_chain([&](read_buffer buffer) {
        while (index < (int32_t)length && buffer.position() < buffer.size()) {
            int32_t err = 0;
            auto value = varint_decode(buffer.cursor(), buffer.available(), &err);
            if (err < 0) {
                return err;
            }

            auto needed = varint_encoding_length(value);

            buffer.skip(needed);
            values[index] = value;
            index++;
        }
        return index;
    });
}

} // namespace phylum
