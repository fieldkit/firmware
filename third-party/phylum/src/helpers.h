#pragma once

#include "data_chain.h"

namespace phylum {

class data_chain_helpers {

public:
    template <typename tree_type>
    static int32_t indexed_seek(data_chain &chain, tree_ptr_t ptr, uint32_t desired_position) {
        int32_t err;

        tree_type position_index{ chain.pc(), ptr, "pos-idx" };

        uint32_t found_position = 0;
        uint32_t found_sector = InvalidSector;

        err = position_index.find_last_less_then(desired_position, &found_sector, &found_position);
        if (err < 0) {
            return err;
        }

        phydebugf("seeking desired=%d found-position=%d found-sector=%d", desired_position, found_position,
                  found_sector);

        assert(desired_position >= found_position);

        err = chain.seek_sector(found_sector, found_position, desired_position);
        if (err < 0) {
            return err;
        }

        return chain.cursor().position;
    }
};

} // namespace phylum
