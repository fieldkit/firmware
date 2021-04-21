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

    return buffer_.fill(data, size, [&](simple_buffer &) -> int32_t {
        return flush();
    });
}

int32_t file_appender::make_data_chain() {
    logged_task lt{ "fa-mdc" };

    auto err = data_chain_.create_if_necessary();
    if (err < 0) {
        return err;
    }

    err = directory_->read(file_.id, [&](read_buffer data_buffer) -> int32_t {
        return data_buffer.read_to_end([&](read_buffer rb) -> int32_t {
            return data_chain_.write(rb.ptr(), rb.size());
        });
    });
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
    }

    return 0;
}

int32_t file_appender::index_if_necessary(std::function<int32_t(data_chain_cursor)> fn) {
    auto cursor = this->cursor();
    assert(cursor.sector != InvalidSector);

    if (cursor.position == 0 || (data_chain_.visited_sectors() > 0 && data_chain_.visited_sectors() % 16 == 0)) {
        auto err = fn(cursor);
        if (err < 0) {
            return err;
        }

        data_chain_.clear_visited_sectors();
    }

    return 0;
}

int32_t file_appender::flush() {
    logged_task lt{ "fa-flush" };

    assert(file_.id != UINT32_MAX);

    // Do we already have a data chain?
    auto had_chain = data_chain_.valid();
    if (had_chain) {
        phydebugf("writing to chain");

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
            phyinfof("flush: inline id=0x%x bytes=%zu begin", file_.id, pending);

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

            phyinfof("flush: inline done");
        } else {
            phyinfof("flush making chain (%d)", buffer_.position());

            auto err = make_data_chain();
            if (err < 0) {
                phyerrorf("mdc failed");
                return err;
            }

            phyinfof("flush making chain done (%d)", buffer_.position());
        }
    }

    if (data_chain_.valid()) {
        phydebugf("flush remaining (%d)", buffer_.position());

        auto err = data_chain_.flush();
        if (err < 0) {
            return err;
        }

        if (!had_chain) {
            phyinfof("%s updating directory", data_chain_.name());
            file_.chain.head = data_chain_.head();
            file_.chain.tail = data_chain_.tail();
            auto err = directory_->file_chain(file_.id, file_.chain);
            if (err < 0) {
                return err;
            }

            return 0;
        }
    }

    return 0;
}

uint32_t file_appender::u32(uint8_t type) {
    assert(file_.cfg.nattrs > 0);
    for (auto i = 0u; i < file_.cfg.nattrs; ++i) {
        auto &attr = file_.cfg.attributes[i];
        if (attr.type == type) {
            assert(sizeof(uint32_t) == attr.size);
            return *(uint32_t *)attr.ptr;
        }
    }
    assert(false);
    return 0;
}

void file_appender::u32(uint8_t type, uint32_t value) {
    assert(file_.cfg.nattrs > 0);
    for (auto i = 0u; i < file_.cfg.nattrs; ++i) {
        auto &attr = file_.cfg.attributes[i];
        if (attr.type == type) {
            assert(sizeof(uint32_t) == attr.size);
            if (*(uint32_t *)attr.ptr != value) {
                *(uint32_t *)attr.ptr = value;
                attr.dirty = true;
            }
            return;
        }
    }
    assert(false);
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

    return 0;
}

} // namespace phylum
