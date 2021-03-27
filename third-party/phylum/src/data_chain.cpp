#include "data_chain.h"

namespace phylum {

int32_t data_chain::write_header() {
    logged_task{ "dc-write-hdr", name() };

    assert_valid();

    db().emplace<data_chain_header_t>();

    db().terminate();

    dirty(true);
    appendable(true);

    return 0;
}

int32_t data_chain::seek_end_of_buffer() {
    auto err = db().seek_end();
    if (err < 0) {
        return err;
    }

    appendable(true);

    return 0;
}

int32_t data_chain::write(uint8_t const *data, size_t size) {
    logged_task{ "dc-write", name() };

    auto copied = 0u;
    return write_chain([&](simple_buffer &buffer, bool &grow) {
        auto remaining = size - copied;
        auto copying = std::min<int32_t>(buffer.available(), remaining);
        if (copying > 0) {
            memcpy(buffer.cursor(), data + copied, copying);
            copied += copying;
            buffer.skip(copying);
        }
        if (size - copied > 0) {
            grow = true;
        }
        return copying;
    });
}

int32_t data_chain::read(uint8_t *data, size_t size) {
    logged_task lt{ "dc-read", name() };

    assert_valid();

    simple_buffer reading{ data, size };
    return read_chain([&](simple_buffer &view) {
        return reading.fill_from(view);
    });
}

uint32_t data_chain::total_bytes() {
    logged_task lta{ "total-bytes" };

    back_to_head();

    auto bytes = 0u;
    while (forward() > 0) {
        bytes += header<data_chain_header_t>()->bytes;
    }

    phydebugf("done (%d)", bytes);

    return bytes;
}

int32_t data_chain::seek(file_size_t /*position*/, seek_reference /*reference*/) {
    return 0;
}

} // namespace phylum
