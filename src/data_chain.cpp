#include "phylum.h"
#include "data_chain.h"

namespace phylum {

int32_t data_chain::write_header(page_lock &page_lock) {
    logged_task it{ "dc-write-hdr", name() };

    assert_valid();

    db().emplace<data_chain_header_t>();

    db().terminate();

    page_lock.dirty();
    appendable(true);

    return 0;
}

int32_t data_chain::seek_sector(dhara_sector_t new_sector, file_size_t position_at_start_of_sector, file_size_t desired_position) {
    assert(desired_position >= position_at_start_of_sector);

    sector(new_sector);

    position_ = position_at_start_of_sector;
    position_at_start_of_sector_ = position_at_start_of_sector;

    auto nread = skip_bytes(desired_position == UINT32_MAX ? desired_position : desired_position - position_);
    if (nread < 0) {
        return nread;
    }

    phydebugf("seek done desired=%d position=%d nread=%d", desired_position, position_, nread);

    assert(desired_position == InvalidSector || desired_position == position_);

    return nread;
}

int32_t data_chain::skip_bytes(file_size_t bytes) {
    int32_t nread = 0;

    while (true) {
        auto remaining = MaximumNullReadSize;
        if (bytes != UINT32_MAX) {
            remaining = bytes - nread;
        }
        if (remaining == 0) {
            break;
        }

        auto err = read(nullptr, remaining);
        if (err < 0) {
            return err;
        }
        if (err == 0) {
            break;
        }

        nread += err;
    }

    return nread;
}

int32_t data_chain::skip_records(record_number_t number_records) {
    int32_t err;

    record_number_t skipped = 0;

    while (skipped < number_records) {
        uint32_t record_size = 0;
        err = read_delimiter(&record_size);
        if (err < 0) {
            return err;
        }

        if (err == 0) {
            break;
        }

        phydebugf("%d record-size: %d (%d delim-bytes)", position_, record_size, err);

        err = skip_bytes(record_size);
        if (err < 0) {
            return err;
        }

        skipped++;
    }

    return skipped;
}

int32_t data_chain::seek_end_of_buffer(page_lock &/*page_lock*/) {
    auto err = db().seek_end();
    if (err < 0) {
        return err;
    }

    phydebugf("seeking end of buffer %d", db().position());

    appendable(true);

    return 0;
}

int32_t data_chain::write(uint8_t const *data, size_t size) {
    logged_task it{ "dc-write", name() };

    auto copied = 0u;
    return write_chain([&](write_buffer buffer, bool &grow) {
        auto remaining = size - copied;
        auto copying = std::min<int32_t>(buffer.available(), remaining);
        if (copying > 0) {
            memcpy(buffer.cursor(), data + copied, copying);
            copied += copying;
        }
        if (size - copied > 0) {
            grow = true;
        }
        return copying;
    });
}

int32_t data_chain::read_delimiter(uint32_t *delimiter) {
    assert(delimiter != nullptr);

    logged_task lt{ "dc-read", name() };

    assert_valid();

    *delimiter = 0;

    int32_t bits = 0;
    int32_t nread = 0;

    return read_chain([&](read_buffer view) -> int32_t {
        uint8_t byte;

        while (true) {
            auto err = view.read_byte(&byte);
            if (err < 0) {
                return err;
            }
            if (err != 1) {
                return -1;
            }

            nread++;

            uint32_t ll = byte;
            *delimiter += ((ll & 0x7F) << bits);
            bits += 7;

            if (!(byte & 0x80)) {
                break;
            }
        }

        return nread;
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

file_size_t data_chain::total_bytes() {
    logged_task lt{ "total-bytes" };

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

    // TODO Can we remove this?
    if (page_lock.is_dirty()) {
        auto err = page_lock.flush(page_lock.sector());
        if (err < 0) {
            return err;
        }
    }

    return written;
}

int32_t data_chain::constrain() {
    auto iter = db().begin();
    auto hdr = db().header<data_chain_header_t>();
    auto total = hdr->bytes + iter.position() + iter.size_of_record() + 1 /* Null terminator */;
    phydebugf("constrain hdr-bytes=%d + hdr-pos=%d + hdr->size=%d + 1 <null> = total=%d", hdr->bytes, iter.position(), iter.size_of_record(), total);
    assert(db().constrain(total) >= 0);
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
