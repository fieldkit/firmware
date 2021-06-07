#include "flat_attribute_storage.h"
#include "data_chain.h"

namespace phylum {

struct attribute_header_t {
    uint8_t type{ 0xff };
    uint8_t size{ 0xff };
    uint8_t reserved[2] = { 0xff, 0xff };
};

flat_attribute_storage::flat_attribute_storage(phyctx pc) : buffers_(&pc.buffers_), sectors_(&pc.sectors_), allocator_(&pc.allocator_) {
}

int32_t flat_attribute_storage::read(tree_ptr_t &ptr, file_id_t /*id*/, open_file_config file_cfg) {
    data_chain chain{ pc(), head_tail_t{ ptr.root, ptr.tail } };

    phyverbosef("reading attributes");

    while (true) {
        attribute_header_t header;

        auto err = chain.read((uint8_t *)&header, sizeof(attribute_header_t));
        if (err < 0) {
            return err;
        }
        if (err != sizeof(attribute_header_t)) {
            break;
        }

        logged_task lt{ chain.name() };

        phydebugf("attribute: type=%d size=%d", header.type, header.size);

        for (auto i = 0u; i < file_cfg.nattrs; ++i) {
            auto &attr = file_cfg.attributes[i];
            if (attr.type == header.type) {
                assert(attr.size == header.size);
                auto err = chain.read((uint8_t *)attr.ptr, attr.size);
                if (err < 0) {
                    return err;
                }
            }
        }
    }

    return 0;
}

int32_t flat_attribute_storage::update(tree_ptr_t &ptr, file_id_t /*id*/, open_file_attribute *attributes, size_t nattrs) {
    auto attribute_size = 0u;
    for (auto i = 0u; i < nattrs; ++i) {
        attribute_size += attributes[i].size;
    }

    assert(attribute_size <= AttributeCapacity);

    head_tail_t head_tail{ ptr.root, ptr.tail };

    phyverbosef("saving attributes total-size=%zu head=%d tail=%d", attribute_size, head_tail.head, head_tail.tail);

    data_chain chain{ pc(), head_tail };

    if (!ptr.valid()) {
        auto err = chain.create_if_necessary();
        if (err < 0) {
            return err;
        }
    }

    auto buffer = buffers_->allocate(buffers_->buffer_size());

    auto wrote = 0u;
    for (auto i = 0u; i < nattrs; ++i) {
        auto &attr = attributes[i];

        attribute_header_t header;
        header.type = attr.type;
        header.size = attr.size;

        auto err = buffer.write((uint8_t *)&header, sizeof(attribute_header_t));
        if (err < 0) {
            return err;
        }

        err = buffer.write((uint8_t *)attr.ptr, attr.size);
        if (err < 0) {
            return err;
        }
    }

    auto err = chain.truncate(buffer.ptr(), buffer.position());
    if (err < 0) {
        return err;
    }

    wrote += err;

    ptr = tree_ptr_t{ chain.head(), chain.tail() };

    phydebugf("saved attributes bytes=%zu head=%d tail=%d", wrote, chain.head(), chain.tail());

    return wrote;
}

} // namespace phylum
