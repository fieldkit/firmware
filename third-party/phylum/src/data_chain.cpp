#include "phylum.h"
#include "data_chain.h"

namespace phylum {

int32_t data_chain::write_header(page_lock &page_lock) {
    logged_task{ "dc-write-hdr", name() };

    assert_valid();

    db().emplace<data_chain_header_t>();

    db().terminate();

    page_lock.dirty();
    appendable(true);

    return 0;
}

int32_t data_chain::seek_end_of_buffer(page_lock &/*page_lock*/) {
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
    return write_chain([&](simple_buffer buffer, bool &grow) {
        auto remaining = size - copied;
        auto copying = std::min<int32_t>(buffer.available(), remaining);
        if (copying > 0) {
            memcpy(buffer.cursor(), data + copied, copying);
            copied += copying;
            // buffer.skip(copying);
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
    return read_chain([&](read_buffer view) {
        return reading.fill_from(view);
    });
}

uint32_t data_chain::total_bytes() {
    logged_task lta{ "total-bytes" };

    auto page_lock = db().reading(head());

    back_to_head(page_lock);

    auto bytes = 0u;
    do {
        bytes += db().header<data_chain_header_t>()->bytes;
    }
    while (forward(page_lock) > 0);

    phydebugf("done (%d)", bytes);

    return bytes;
}

int32_t data_chain::seek(file_size_t /*position*/, seek_reference /*reference*/) {
    return 0;
}

int32_t data_chain::write_chain(std::function<int32_t(write_buffer, bool &)> data_fn) {
    logged_task lt{ "write-data-chain" };

    if (!appendable()) {
        phydebugf("making appendable");

        auto page_lock = db().writing(head());

        assert(back_to_head(page_lock) >= 0);

        logged_task lt{ name() };

        auto err = seek_end_of_chain(page_lock);
        if (err < 0) {
            return err;
        }

        err = write_header_if_at_start(page_lock);
        if (err < 0) {
            return err;
        }

        if (err == 0) {
            auto hdr = db().header<data_chain_header_t>();
            phydebugf("write resuming sector-bytes=%d", hdr->bytes);
            assert(db().skip(hdr->bytes) >= 0);
        }

        appendable(true);
    }

    auto page_lock = db().writing(sector());

    auto written = 0;

    while (true) {
        phydebugf("write: position=%zu available=%zu size=%zu", db().position(), db().available(), db().size());

        auto grow = false;
        auto err = db().write_view([&](write_buffer wb) {
            auto err = data_fn(std::move(wb), grow);
            if (err < 0) {
                return err;
            }

            // Do this before we grow so the details are saved.
            assert(db().write_header<data_chain_header_t>([&](data_chain_header_t *header) {
                assert(header->bytes + err <= (int32_t)sector_size());
                header->bytes += err;
                return 0;
            }) == 0);
            written += err;
            position_ += err;
            db().skip(err); // TODO Remove

            return err;
        });
        if (err < 0) {
            return err;
        }

        page_lock.dirty();

        // Grow and write header.
        if (grow) {
            auto err = grow_tail(page_lock);
            if (err < 0) {
                return err;
            }

            position_at_start_of_sector_ = position_;
        } else {
            if (err == 0) {
                break;
            }
        }
    }
    return written;
}


int32_t data_chain::constrain() {
    auto iter = db().begin();
    auto hdr = db().header<data_chain_header_t>();
    assert(db().constrain(hdr->bytes + iter->position() + iter->size_of_record() + 1) >= 0);
    return 0;
}

int32_t data_chain::read_chain(std::function<int32_t(read_buffer)> data_fn) {
    logged_task lt{ "read-data-chain", name() };

    assert_valid();

    auto page_lock = db().reading(sector());

    auto err = ensure_loaded(page_lock);
    if (err < 0) {
        return err;
    }

    assert(constrain() >= 0);

    while (true) {
        // If we're at the start of the buffer, seek past the
        // data_chain_header so we can read the number of bytes in
        // this sector.
        if (db().position() == 0) {
            auto err = db().seek_end();
            if (err < 0) {
                return err;
            }

            db().skip(1); // HACK TODO Skip terminator.

            assert(constrain() >= 0);

            phydebugf("read resuming position=%d available=%d", db().position(), db().available());
        }

        // If we have data available.
        if (db().available() > 0) {
            phydebugf("view position=%zu available=%zu", db().position(), db().available());

            auto err = data_fn(db().to_read_buffer());
            if (err < 0) {
                return err;
            }

            if (err >= 0) {
                position_ += err;
                db().skip(err); // TODO Remove
                return err;
            }
        }

        auto err = forward(page_lock);
        if (err < 0) {
            return err;
        } else if (err == 0) {
            break;
        }

        position_at_start_of_sector_ = position_;
    }

    return 0;
}

} // namespace phylum
