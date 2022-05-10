#include "data_chain.h"
#include "phylum.h"

namespace phylum {

int32_t data_chain::write_header(page_lock &lock) {
    assert_valid();

    db().clear();

    db().emplace<data_chain_header_t>();

    db().terminate();

    lock.dirty();
    appendable(true);

    return 0;
}

int32_t data_chain::seek_sector(dhara_sector_t new_sector, file_size_t position_at_start_of_sector,
                                file_size_t desired_position) {
    logged_task lt{ "dc-seek", name() };

    assert(desired_position >= position_at_start_of_sector);

    sector(new_sector);

    position_ = position_at_start_of_sector;
    position_at_start_of_sector_ = position_at_start_of_sector;
    auto skipping = desired_position == UINT32_MAX ? desired_position : desired_position - position_;

    auto nread = skip_bytes(skipping);
    if (nread < 0) {
        return nread;
    }

    phyinfof("seek done desired=%d position=%d nread=%d", desired_position, position_, nread);

    assert(desired_position == UINT32_MAX || desired_position == position_);

    if (desired_position == UINT32_MAX) {
        appendable(true);
    }

    return nread;
}

int32_t data_chain::skip_bytes(file_size_t bytes) {
    if (bytes == 0) {
        return bytes;
    }

    int32_t nread = 0;

    noop_writer dev_null{ bytes };

    while (true) {
        auto err = read_chain(dev_null);
        if (err < 0) {
            phyerrorf("skip-bytes: read-chain failed (%d/%d)", nread, bytes);
            return err;
        }
        if (err == 0) {
            break;
        }

        nread += err;
    }

    return nread;
}

int32_t data_chain::skip_records(record_number_t skipping) {
    int32_t err;

    record_number_t records = 0;
    file_size_t bytes = 0;

    while (records < skipping) {
        uint32_t record_size = 0;
        err = read_delimiter(&record_size);
        if (err < 0) {
            phyerrorf("skip-records: read-delimiter failed (%d/%d) (%d bytes so far)", records, skipping, bytes);
            return err;
        }

        if (err == 0) {
            break;
        }

        err = skip_bytes(record_size);
        if (err < 0) {
            phyerrorf("skip-records: skip-bytes failed (%d/%d) (%d bytes so far) (%d bytes in record)", records,
                      skipping, bytes, record_size);
            return err;
        }

        bytes += err + record_size;
        records++;
    }

    phydebugf("skipped records=%d bytes=%d position=%d", records, bytes, position_);

    return records;
}

int32_t data_chain::seek_end_of_buffer(page_lock & /*lock*/) {
    auto err = db().seek_end();
    if (err < 0) {
        return err;
    }

    phyverbosef("seeking end of buffer %d", db().position());

    appendable(true);

    return 0;
}

int32_t data_chain::write(uint8_t const *data, size_t size) {
    logged_task it{ "dc-write", name() };

    read_buffer buffer{ data, size };
    buffer_reader reader{ buffer };
    return write_chain(reader);
}

int32_t data_chain::truncate(uint8_t const *data, size_t size) {
    assert(head() != InvalidSector && tail() != InvalidSector);

    sector(head());

    auto lock = db().writing(sector());

    auto err = prepare_sector(lock, InvalidSector, true);
    if (err < 0) {
        return err;
    }

    read_buffer buffer{ data, size };
    buffer_reader reader{ buffer };
    err = write_chain(lock, reader);
    if (err < 0) {
        return err;
    }

    err = flush(lock);
    if (err < 0) {
        return err;
    }

    return size;
}

/**
 * This would mean very very large blocks of data. Only really around to give us
 * a early, fixed breaking condition below.
 */
constexpr int32_t MaximumDelimiterLength = 32;

int32_t data_chain::read_delimiter(uint32_t *delimiter) {
    assert(delimiter != nullptr);

    assert_valid();

    varint_decoder decoder;
    while (!decoder.done()) {
        auto err = read_chain(decoder);
        if (err < 0) {
            phyerrorf("read-delimiter: read-chain failed");
            return err;
        }
        if (err == 0) {
            if (decoder.bytes_read() > 0) {
                phyerrorf("read-delimiter: no-more-data");
                return -1;
            }
            return 0;
        }

        if (decoder.bytes_read() > MaximumDelimiterLength) {
            phyerrorf("read-delimiter: maximum length reached");
            return -1;
        }
    }

    *delimiter = decoder.value();

    return decoder.bytes_read();
}

int32_t data_chain::read(uint8_t *data, size_t size) {
    logged_task lt{ "dc-read", name() };

    assert_valid();

    write_buffer buffer{ data, size };
    buffer_writer writer{ buffer };
    return read_chain(writer);
}

file_size_t data_chain::total_bytes() {
    logged_task lt{ "total-bytes", name() };

    auto lock = db().reading(head());

    back_to_head(lock);

    auto bytes = 0u;
    do {
        bytes += db().header<data_chain_header_t>()->bytes;
    } while (forward(lock) > 0);

    phyverbosef("done (%d)", bytes);

    return bytes;
}

int32_t data_chain::write_chain(io_reader &reader) {
    if (!appendable()) {
        phyverbosef("making appendable");

        auto lock = db().writing(head());

        assert(back_to_head(lock) >= 0);

        // logged_task lt{ name() };

        auto err = seek_end_of_chain(lock);
        if (err < 0) {
            return err;
        }

        err = write_header_if_at_start(lock);
        if (err < 0) {
            return err;
        }

        if (err == 0) {
            auto hdr = db().header<data_chain_header_t>();
            phyverbosef("write resuming sector-bytes=%d", hdr->bytes);
            assert(db().skip(hdr->bytes) >= 0);
        }

        appendable(true);
    }

    auto lock = db().writing(sector());

    auto err = write_chain(lock, reader);
    if (err < 0) {
        return err;
    }

    return err;
}

int32_t data_chain::write_chain(page_lock &lock, io_reader &reader) {
    auto written = 0;

    while (true) {
        phyverbosef("write: position=%zu available=%zu size=%zu", db().position(), db().available(), db().size());

        auto grow = false;
        auto err = db().write_view([&](write_buffer wb) {
            auto bytes_read = reader.read(wb.cursor(), wb.available());
            if (bytes_read < 0) {
                return bytes_read;
            }

            // Do this before we grow so the details are saved.
            auto err = db().write_header<data_chain_header_t>([&](data_chain_header_t *header) {
                assert(header->bytes + bytes_read <= (int32_t)sector_size());
                header->bytes += bytes_read;
                return 0;
            });
            assert(err == 0);

            written += bytes_read;
            position_ += bytes_read;

            db().skip(bytes_read); // TODO Remove

            if ((int32_t)wb.available() == bytes_read) {
                grow = true;
            }
            return bytes_read;
        });
        if (err < 0) {
            return err;
        }

        lock.dirty();

        // Grow and write header.
        if (grow) {
            auto err = grow_tail(lock);
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
    if (lock.is_dirty()) {
        auto err = lock.flush(lock.sector());
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
    phyverbosef("constrain hdr-bytes=%d + hdr-pos=%d + hdr->size=%d + 1 <null> = total=%d", hdr->bytes, iter.position(),
                iter.size_of_record(), total);
    assert(db().constrain(total) >= 0);

    /**
     * Due to a bug somewhere, if the sector is empty, then the
     * delimited buffer position could be just before the null
     * terminator. This seems like an ok sanity check, that the
     * position should never be before the minimum position and will
     * hold us over until I can find the real off by one issue.
     */
    auto minimum = 2 + sizeof(data_chain_header_t) + 1;
    if (db().position() < minimum) {
        phyverbosef("constraining to minimum position=%d", minimum);
        db().position(minimum);
    }
    return 0;
}

int32_t data_chain::read_chain(io_writer &writer) {
    assert_valid();

    auto lock = db().reading(sector());

    auto err = ensure_loaded(lock);
    if (err < 0) {
        phyerrorf("read-chain: load fail");
        return err;
    }

    assert(constrain() >= 0);

    auto nread_this_call = 0u;

    while (true) {
        // If we're at the start of the buffer, seek past the
        // data_chain_header so we can read the number of bytes in
        // this sector.
        if (db().position() == 0) {
            auto err = db().seek_end();
            if (err < 0) {
                phyerrorf("read-chain: seek-end fail");
                return err;
            }

            assert(constrain() >= 0);

            phyverbosef("read resuming position=%d available=%d", db().position(), db().available());
        }

        // If we have data available.
        if (db().available() > 0) {
            auto read_buffer = db().to_read_buffer();

            phyverbosef("view position=%zu available=%zu readable=%d", db().position(), db().available(),
                        read_buffer.available());

            auto err = writer.write(read_buffer.cursor(), read_buffer.available());
            if (err < 0) {
                phyerrorf("read-chain: write fail (%d bytes-read-this-call) (%d)", nread_this_call, err);
                return err;
            }

            if (err >= 0) {
                position_ += err;
                db().skip(err); // TODO Remove
                return err;
            }
        }

        auto err = forward(lock);
        if (err < 0) {
            phyerrorf("read-chain: forward fail");
            return err;
        } else if (err == 0) {
            break;
        }

        position_at_start_of_sector_ = position_;
    }

    return 0;
}

} // namespace phylum
