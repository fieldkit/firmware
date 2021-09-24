#include "super_chain.h"

namespace phylum {

int32_t super_chain::mount() {
    auto page_lock = db().writing(InvalidSector);

    auto err = mount_chain(page_lock);
    if (err < 0) {
        return err;
    }

    auto hdr = db().header<super_block_t>();

    directory_tree_ = hdr->directory_tree;

    return 0;
}

int32_t super_chain::format() {
    auto page_lock = db().writing(head());

    auto err = create_chain(page_lock);
    if (err < 0) {
        return err;
    }

    return 0;
}

int32_t super_chain::update(tree_ptr_t directory_tree) {
    auto page_lock = db().writing(head());
    auto modified = false;

    assert(db().write_header<super_block_t>([&](super_block_t *header) {
        if (header->directory_tree != directory_tree) {
            header->directory_tree = directory_tree;
            modified = true;
        }
        return 0;
    }) == 0);

    if (modified) {
        phyinfof("saving super block");

        page_lock.dirty();

        return flush(page_lock);
    }

    return 0;
}

int32_t super_chain::write_header(page_lock &page_lock) {
    db().emplace<super_block_t>();

    page_lock.dirty();

    return 0;
}

}
