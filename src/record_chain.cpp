#include "record_chain.h"

namespace phylum {

int32_t record_chain::mount_chain(page_lock &page_lock) {
    logged_task lt{ "mount" };

    sector(head());

    dhara_page_t page = 0;
    auto find = sectors()->find(0, &page);
    if (find < 0) {
        return find;
    }

    auto err = page_lock.replace(sector());
    if (err < 0) {
        return err;
    }

    err = load(page_lock);
    if (err < 0) {
        return err;
    }

    back_to_head(page_lock);

    phyinfof("mounted %d", sector());

    return 0;
}

int32_t record_chain::create_chain(page_lock &page_lock) {
    logged_task lt{ "create" };

    sector(head());

    phyinfof("creating");
    auto err = write_header(page_lock);
    if (err < 0) {
        return err;
    }

    page_lock.dirty();
    appendable(true);

    err = flush(page_lock);
    if (err < 0) {
        return err;
    }

    return 0;
}

int32_t record_chain::seek_end_of_buffer(page_lock &/*page_lock*/) {
    return db().seek_end();
}

int32_t record_chain::prepare(page_lock &page_lock, size_t required) {
    logged_task lt{ "prepare" };

    if (!appendable()) {
        auto err = seek_end_of_chain(page_lock);
        if (err < 0) {
            phyerrorf("seek-end failed");
            return err;
        }
    }

    auto err = grow_if_necessary(page_lock, required);
    if (err < 0) {
        phyerrorf("grow failed");
        return err;
    }

    // assert(db().header<directory_chain_header_t>()->type == entry_type::DirectorySector);

    page_lock.dirty();
    appendable(true);

    return 0;
}

int32_t record_chain::grow_if_necessary(page_lock &page_lock, size_t required) {
    auto delimiter_overhead = varint_encoding_length(required);
    auto total_required = delimiter_overhead + required;
    if (db().room_for(total_required)) {
        return 0;
    }

    return grow_tail(page_lock);
}

} // namespace phylum
