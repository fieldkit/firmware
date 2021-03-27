#include "file_reader.h"

namespace phylum {

file_reader::file_reader(directory_chain &directory, found_file file, simple_buffer &&buffer)
    : directory_(directory), file_(file), buffer_(std::move(buffer)), data_chain_(directory, file.chain) {
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
    assert(size >= file_.size);

    auto err = directory_.read(file_.id, data, size);
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

uint32_t file_reader::u32(uint8_t /*type*/) {
    return -1;
}

} // namespace phylum
