#pragma once

#include "sector_chain.h"

namespace phylum {

class record_chain : public sector_chain {
public:
    record_chain(phyctx pc, head_tail_t chain, const char *prefix)
        : sector_chain(pc, chain, prefix) {
    }

protected:
    int32_t mount_chain(page_lock &page_lock);

    int32_t create_chain(page_lock &page_lock);

    int32_t prepare(page_lock &page_lock, size_t required);

    int32_t grow_if_necessary(page_lock &page_lock, size_t required);

    int32_t seek_end_of_buffer(page_lock &page_lock) override;

    template <typename T, class... Args>
    int32_t emplace(page_lock &page_lock, Args &&... args) {
        // NOTE Buffer has 0 size until paged in.
        // assert(sizeof(T) <= db().size());

        auto err = prepare(page_lock, sizeof(T));
        if (err < 0) {
            return err;
        }

        // logged_task lt{ name() };

        db().emplace<T, Args...>(std::forward<Args>(args)...);

        page_lock.dirty();

        return 0;
    }

    template <typename T>
    int32_t append(page_lock &page_lock, T &record, uint8_t const *buffer, size_t size) {
        // NOTE Buffer has 0 size until paged in.
        // assert(sizeof(T) <= db().size());

        auto err = prepare(page_lock, sizeof(T) + size);
        if (err < 0) {
            return err;
        }

        // logged_task lt{ name() };

        db().append<T>(record, buffer, size);

        page_lock.dirty();

        return 0;
    }
};

} // namespace phylum
