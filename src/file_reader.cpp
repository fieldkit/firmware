#include "file_reader.h"

namespace phylum {

file_reader::file_reader(sector_chain &other, directory *directory, found_file file)
    : directory_(directory), file_(file), buffer_(std::move(other.buffers().allocate(other.buffers().buffer_size()))),
      data_chain_(other, file.chain, "file-rdr") {
}

file_reader::file_reader(working_buffers &buffers, sector_map &sectors, sector_allocator &allocator, directory *directory, found_file file)
    : directory_(directory), file_(file), buffer_(std::move(buffers.allocate(sectors.sector_size()))),
      data_chain_(buffers, sectors, allocator, file.chain, "file-rdr") {
}

file_reader::~file_reader() {
}

int32_t file_reader::read(uint8_t *data, size_t size) {
    if (has_chain()) {
        auto err = data_chain_.read(data, size);
        if (err < 0) {
            return err;
        }

        position_ += err;

        return err;
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

    position_ += err;

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
