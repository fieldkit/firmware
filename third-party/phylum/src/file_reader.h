#pragma once

#include "data_chain.h"
#include "simple_buffer.h"
#include "directory.h"

namespace phylum {

class file_reader {
private:
    phyctx pc_;
    directory *directory_{ nullptr };
    found_file file_;
    simple_buffer buffer_;
    data_chain data_chain_;
    file_size_t inline_position_{ 0 };

public:
    file_reader(phyctx pc, directory *directory, found_file file);

    virtual ~file_reader();

public:
    file_size_t position() const;

public:
    int32_t read(uint8_t *data, size_t size);

    int32_t read(size_t size);

    uint32_t u32(uint8_t type);

    int32_t close();

public:
    template <typename tree_type>
    int32_t seek_position(uint32_t desired_position) {
        int32_t err;

        tree_type position_index{ data_chain_.pc(), file_.position_index, "posidx" };

        uint32_t found_position = 0;
        uint32_t found_sector = 0;

        position_index.log();

        err = position_index.find_last_less_then(desired_position, &found_sector, &found_position);
        if (err < 0) {
            return err;
        }

        phydebugf("seeking desired=%d found-position=%d found-sector=%d", desired_position, found_position, found_sector);

        assert(desired_position >= found_position);

        err = data_chain_.seek_sector(found_sector, found_position, desired_position);
        if (err < 0) {
            return err;
        }

        return position();
    }

    template <typename tree_type>
    int32_t seek_record(record_number_t desired_record) {
        int32_t err;

        // TODO Skip this if desired_record == 0

        tree_type record_index{ data_chain_.pc(), file_.record_index, "recidx" };

        uint32_t found_record = 0;
        uint32_t found_record_position = 0;

        record_index.log();

        err = record_index.find_last_less_then(desired_record, &found_record_position, &found_record);
        if (err < 0) {
            return err;
        }

        phydebugf("seeking record desired=%d found-record=%d found-position=%d", desired_record, found_record, found_record_position);

        err = seek_position<tree_type>(found_record_position);
        if (err < 0) {
            return err;
        }

        phydebugf("skipping %d records position=%d", desired_record, err);

        if (desired_record == UINT32_MAX) {
            err = data_chain_.skip_records(desired_record);
        }
        else {
            err = data_chain_.skip_records(desired_record - found_record);
        }
        if (err < 0) {
            return err;
        }

        phydebugf("done skipping %d", err);

        return found_record + err;
    }

private:
    bool has_chain() const {
        return data_chain_.valid();
    }
};

} // namespace phylum
