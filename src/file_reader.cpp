#include "file_reader.h"

namespace phylum {

file_reader::file_reader(phyctx pc, directory *directory, found_file file)
    : pc_(pc), directory_(directory), file_(file), data_chain_(pc, file.chain, "file-rdr") {
}

file_reader::~file_reader() {
}

file_size_t file_reader::position() const {
    if (has_chain()) {
        auto dcc = data_chain_.cursor();
        return dcc.position;
    }
    return inline_position_;
}

int32_t file_reader::read(size_t size) {
    return read(nullptr, size);
}

int32_t file_reader::read(uint8_t *data, size_t size) {
    logged_task lt{ "fr-read" };

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
    buffer_writer writer{ filling };

    auto err = directory_->read(file_.id, writer);
    if (err < 0) {
        return err;
    }

    inline_position_ += err;

    return err;
}

int32_t file_reader::close() {
    return 0;
}

} // namespace phylum
