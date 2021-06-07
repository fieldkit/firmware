#include "file_appender.h"

namespace phylum {

file_appender::file_appender(phyctx pc, directory *directory, found_file file)
    : pc_(pc), directory_(directory), file_(file), buffer_(std::move(pc.buffers_.allocate(pc.sectors_.sector_size()))),
      data_chain_(pc, file.chain, "file-app") {
}

file_appender::~file_appender() {
}

size_t file_appender::visited_sectors() {
    if (has_chain()) {
        return data_chain_.visited_sectors();
    }
    return 0;
}

data_chain_cursor file_appender::cursor() {
    if (has_chain()) {
        auto dcc = data_chain_.cursor();
        return data_chain_cursor{
            dcc.sector, dcc.position + (file_size_t)buffer_.position(),
            dcc.position_at_start_of_sector
        };
    }
    return data_chain_cursor{};
}

file_size_t file_appender::position() {
    if (has_chain()) {
        return cursor().position;
    }
    return file_.directory_size + buffer_.position();
}

int32_t file_appender::write(uint8_t const *data, size_t size) {
    logged_task lt{ "fa-write" };

    phyverbosef("appender-write: position=%d buffer=%d size=%d", cursor().position, buffer_.position(), size);

    auto wrote = buffer_.fill_from_buffer_ptr(data, size, [&](simple_buffer &) -> int32_t {
        auto flushing = buffer_.position();
        auto err = flush();
        if (err < 0) {
            return err;
        }
        return flushing;
    });

    return wrote;
}

int32_t file_appender::make_data_chain() {
    logged_task lt{ "fa-mdc" };

    auto err = data_chain_.create_if_necessary();
    if (err < 0) {
        return err;
    }

    err = directory_->read(file_.id, data_chain_);
    if (err < 0) {
        return err;
    }

    if (buffer_.position() > 0) {
        auto err = buffer_.read_to_position([&](read_buffer rb) -> int32_t {
            return data_chain_.write(rb.ptr(), rb.size());
        });
        if (err < 0) {
            return err;
        }

        buffer_.clear();

        return err;
    }

    return 0;
}

int32_t file_appender::index_necessary() {
    // We use the appender cursor so that we don't keep indexing at
    // the start, while data chain cursor hasn't moved because we're
    // buffering. This doesn't affect anything.
    auto cursor = this->cursor();
    assert(cursor.sector != InvalidSector);

    if (cursor.position == 0 || data_chain_.visited_sectors() > 16) {
        data_chain_.clear_visited_sectors();
        return 1;
    }

    return 0;
}

int32_t file_appender::seek() {
    if (!data_chain_.valid()) {
        phywarnf("noop seek");
        return 0;
    }

    auto err = data_chain_.seek_sector(data_chain_.head(), 0, UINT32_MAX);
    if (err < 0) {
        return err;
    }
    return 0;
}

int32_t file_appender::flush() {
    logged_task lt{ "fa-flush" };

    assert(file_.id != UINT32_MAX);

    // Do we already have a data chain?
    auto had_chain = data_chain_.valid();
    if (had_chain) {
        phyverbosef("writing to chain head=%d", data_chain_.head());

        auto truncated = ((int32_t)file_.cfg.flags & (int32_t)open_file_flags::Truncate) > 0;
        if (!truncated_ && truncated) {
            phyinfof("truncating");
            auto err = data_chain_.truncate();
            if (err < 0) {
                return err;
            }
            truncated_ = true;
        }

        auto err = buffer_.read_to_position([&](read_buffer buffer) -> int32_t {
            return data_chain_.write(buffer.ptr(), buffer.size());
        });
        if (err < 0) {
            return err;
        }

        buffer_.clear();
    } else {
        auto pending = buffer_.position();
        if (pending == 0) {
            return 0;
        }

        if (pending < file_.directory_capacity) {
            phyverbosef("flush: inline id=0x%x bytes=%zu begin", file_.id, pending);

            auto err = buffer_.read_to_position([&](read_buffer buffer) -> int32_t {
                assert(directory_->file_data(file_.id, file_.directory_size, buffer.ptr(), buffer.size()) >= 0);
                return buffer.size();
            });
            if (err < 0) {
                return err;
            }

            file_.directory_capacity -= pending;
            file_.directory_size += pending;

            buffer_.clear();

            phyverbosef("flush: inline done");
        } else {
            phyverbosef("flush making chain (%d)", buffer_.position());

            auto err = make_data_chain();
            if (err < 0) {
                phyerrorf("mdc");
                return err;
            }

            phyverbosef("flush making chain done position=%d err=%d", data_chain_.cursor().position, err);
        }
    }

    if (data_chain_.valid()) {
        phyverbosef("flush remaining (%d)", buffer_.position());

        auto err = data_chain_.flush();
        if (err < 0) {
            return err;
        }

        if (!had_chain) {
            file_.chain.head = data_chain_.head();
            file_.chain.tail = data_chain_.tail();
            phyverbosef("%s updating directory head=%d tail=%d", data_chain_.name(), file_.chain.head, file_.chain.tail);
            auto err = directory_->file_chain(file_.id, file_.chain);
            if (err < 0) {
                return err;
            }

            return 0;
        }
    }

    return 0;
}

int32_t file_appender::close() {
    logged_task lt{ "fa-close" };

    auto err = flush();
    if (err < 0) {
        return err;
    }

    err = directory_->file_attributes(file_.id, file_.cfg.attributes, file_.cfg.nattrs);
    if (err < 0) {
        return err;
    }

    for (auto i = 0u; i < file_.cfg.nattrs; ++i) {
        auto &attr = file_.cfg.attributes[i];
        attr.dirty = false;
    }

    buffer_.free();

    return 0;
}

} // namespace phylum
