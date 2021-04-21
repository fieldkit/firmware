#include "file_reader.h"

namespace phylum {

file_reader::file_reader(phyctx pc, directory *directory, found_file file)
    : pc_(pc), directory_(directory), file_(file), buffer_(std::move(pc.buffers_.allocate(pc.sectors_.sector_size()))),
      data_chain_(pc, file.chain, "file-rdr") {
}

file_reader::~file_reader() {
}

file_size_t file_reader::position() const {
    auto buffer_position = buffer_.position();
    if (has_chain()) {
        auto dcc = data_chain_.cursor();
        return dcc.position + buffer_position;
    }
    return inline_position_;
}

int32_t file_reader::read(size_t size) {
    return read(nullptr, size);
}

int32_t file_reader::read(uint8_t *data, size_t size) {
    if (has_chain()) {
        auto nread = 0u;
        while (nread < size) {
            auto err = data_chain_.read(data == nullptr ? nullptr : data + nread, size - nread);
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

    // Right now all inline data has to be read in a single
    // call. Gotta start somewhere. More often than not this will be
    // the case, can fix later. I'm only doing this check here because
    // we have the file size. There's a similar warning inside
    // directory_chain::read
    assert(size >= file_.directory_size);

    simple_buffer filling{ data, size };

    auto err = directory_->read(file_.id, [&](read_buffer data_buffer) {
        auto f = filling.fill(data_buffer, [](simple_buffer &/*rb*/) {
            phyerrorf("unexpected flush");
            assert(false);
            return -1;
        });
        return f;
    });
    if (err < 0) {
        return err;
    }

    inline_position_ += err;

    return err;
}

int32_t file_reader::close() {
    // There's nothing for us to do, yet. Keeping this call just in case.
    return 0;
}

uint32_t file_reader::u32(uint8_t type) {
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

} // namespace phylum
