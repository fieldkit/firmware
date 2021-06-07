#include "directory_chain.h"

namespace phylum {

int32_t directory_chain::mount() {
    auto page_lock = db().writing(InvalidSector);

    return mount_chain(page_lock);
}

int32_t directory_chain::format() {
    auto page_lock = db().writing(head());

    return create_chain(page_lock);
}

int32_t directory_chain::write_header(page_lock &page_lock) {
    logged_task lt{ "dc-write-hdr", this->name() };

    db().emplace<directory_chain_header_t>();

    page_lock.dirty();

    return 0;
}

int32_t directory_chain::touch(const char *name) {
    logged_task lt{ "dir-touch" };

    auto page_lock = db().writing(sector());

    assert(emplace<file_entry_t>(page_lock, name) >= 0);

    auto err = flush(page_lock);
    if (err < 0) {
        return err;
    }

    return 0;
}

int32_t directory_chain::unlink(const char *name) {
    logged_task lt{ "dir-unlink" };

    auto page_lock = db().writing(sector());

    auto id = make_file_id(name);
    assert(emplace<file_data_t>(page_lock, id, (uint32_t)0) >= 0);

    auto err = flush(page_lock);
    if (err < 0) {
        return err;
    }

    return 0;
}

int32_t directory_chain::file_attribute(page_lock &lock, file_id_t id, open_file_attribute attribute) {
    logged_task lt{ "dir-file-attribute" };

    file_attribute_t fa{ id, attribute.type, attribute.size };
    assert(append<file_attribute_t>(lock, fa, (uint8_t const *)attribute.ptr, attribute.size) >= 0);

    return 0;
}

int32_t directory_chain::file_attributes(file_id_t file_id, open_file_attribute *attributes, size_t nattrs) {
    auto lock = db().writing(sector());

    for (auto i = 0u; i < nattrs; ++i) {
        auto &attr = attributes[i];
        if (attr.dirty) {
            if (attr.size == sizeof(uint32_t)) {
                uint32_t value = *(uint32_t *)attr.ptr;
                phydebugf("attribute[%d] write type=%d size=%d value=0x%x", i, attr.type, attr.size, value);
            } else {
                phydebugf("attribute[%d] write type=%d size=%d", i, attr.type, attr.size);
            }
            assert(file_attribute(lock, file_id, attr) >= 0);
        }
    }

    auto err = lock.flush(lock.sector());
    if (err < 0) {
        return err;
    }

    return 0;
}

int32_t directory_chain::file_chain(file_id_t id, head_tail_t chain) {
    logged_task lt{ "dir-file-chain" };
    auto page_lock = db().writing(sector());

    assert(emplace<file_data_t>(page_lock, id, chain) >= 0);

    assert(file_.id == id);
    file_.chain = chain;

    auto err = flush(page_lock);
    if (err < 0) {
        return err;
    }

    return 0;
}

int32_t directory_chain::file_data(file_id_t id, file_size_t position, uint8_t const *buffer, size_t size) {
    logged_task lt{ "dir-file-data" };
    auto page_lock = db().writing(sector());

    // Detect a truncation. This is fine by me and is in keeping with
    // the design goals of the directory chain implementation in that
    // they be append only. We still need a garbage collection/compaction.
    auto truncated = ((int32_t)file_.cfg.flags & (int32_t)open_file_flags::Truncate) > 0;
    if (position == 0 && truncated) {
        phydebugf("truncating");
        assert(emplace<file_entry_t>(page_lock, id) >= 0);
    }

    phydebugf("appending data");
    file_data_t fd{ id, (uint32_t)size };
    assert(append<file_data_t>(page_lock, fd, buffer, size) >= 0);

    auto err = flush(page_lock);
    if (err < 0) {
        return err;
    }

    return 0;
}

int32_t directory_chain::file_trees(file_id_t /*id*/, tree_ptr_t /*position_index*/, tree_ptr_t /*record_index*/) {
    assert(false);
    return -1;
}

int32_t directory_chain::find(const char *name, open_file_config file_cfg) {
    logged_task lt{ "dir-find" };

    auto id = make_file_id(name);

    file_ = found_file{};
    file_.cfg = file_cfg;

    // Zero attribute values before we scan.
    for (auto i = 0u; i < file_cfg.nattrs; ++i) {
        auto &attr = file_cfg.attributes[i];
        bzero(attr.ptr, attr.size);
    }

    auto err = walk([&](page_lock &/*page_lock*/, entry_t const *entry, record_ptr &record) -> int32_t {
        if (entry->type == entry_type::FileEntry) {
            auto fe = record.as<file_entry_t>();
            if (strncmp(fe->name, name, MaximumNameLength) == 0 || fe->id == id) {
                file_ = found_file{ };
                file_.cfg = file_cfg;
                file_.id = fe->id;
                file_.directory_size = 0;
                file_.directory_capacity = db().size() / 2;
                file_.record = sector_position_t{ sector(), record.position() };
                phydebugf("found file_entry");
            }
        }
        if (entry->type == entry_type::FileData) {
            auto fd = record.as<file_data_t>();
            if (fd->id == file_.id) {
                if (fd->chain.head != InvalidSector || fd->chain.tail != InvalidSector) {
                    file_.directory_size = 0;
                    file_.directory_capacity = 0;
                    file_.chain = fd->chain;
                } else {
                    // Empty FileData is a deletion.
                    if (fd->size == 0) {
                        file_ = found_file{ };
                    }
                    else {
                        if (((int32_t)file_cfg.flags & (int32_t)open_file_flags::Truncate) == 0) {
                            file_.directory_size += fd->size;
                            file_.directory_capacity -= fd->size;
                        }
                    }
                }
            }
        }
        if (entry->type == entry_type::FileAttribute && file_cfg.nattrs > 0) {
            auto fa = record.as<file_attribute_t>();
            if (fa->id == file_.id) {
                for (auto i = 0u; i < file_cfg.nattrs; ++i) {
                    if (fa->type == file_cfg.attributes[i].type) {
                        auto err = record.read_data<file_attribute_t>([&](read_buffer data_buffer) {
                            assert(data_buffer.available() == file_cfg.attributes[i].size);
                            memcpy(file_cfg.attributes[i].ptr, data_buffer.cursor(), data_buffer.available());
                            return data_buffer.available();
                        });
                        if (err < 0) {
                            return err;
                        }
                    }
                }
            }
        }
        return 0;
    });

    if (err < 0) {
        phyerrorf("find failed");
        file_ = found_file{};
        return err;
    }

    if (file_.id == UINT32_MAX) {
        phywarnf("find found no file");
        file_ = found_file{};
        return 0;
    }

    return 1;
}

found_file directory_chain::open() {
    assert(file_.id != UINT32_MAX);
    return file_;
}

int32_t directory_chain::seek_file_entry(file_id_t id) {
    return walk([&](page_lock &/*page_lock*/, entry_t const *entry, record_ptr &record) {
        if (entry->type == entry_type::FileEntry) {
            auto fe = record.as<file_entry_t>();
            if (fe->id == id) {
                appendable(false);
                return 1;
            }
        }
        return 0;
    });
}

int32_t directory_chain::read(file_id_t id, io_writer &writer) {
    auto copied = 0u;
    auto can_read = false;

    auto err = walk([&](page_lock & /*page_lock*/, entry_t const *entry, record_ptr &record) {
        if (entry->type == entry_type::FileEntry) {
            auto spos = sector_position_t{ sector(), record.position() };
            can_read = spos == file_.record;
        }
        if (entry->type == entry_type::FileData && can_read) {
            auto fd = record.as<file_data_t>();
            if (fd->id == id) {
                phydebugf("%s (copy) id=0x%x bytes=%d size=%d", this->name(), fd->id, fd->size, file_.directory_size);

                auto err = record.read_data<file_data_t>([&](read_buffer data_buffer) {
                    return writer.write(data_buffer.ptr(), data_buffer.size());
                });
                if (err < 0) {
                    return err;
                }

                copied += err;
            }
        }
        return (int32_t)0;
    });
    if (err < 0) {
        return err;
    }
    return copied;
}

} // namespace phylum
