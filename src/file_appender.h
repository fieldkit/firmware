#pragma once

#include "data_chain.h"
#include "simple_buffer.h"
#include "directory.h"
#include "writer.h"
#include "helpers.h"

namespace phylum {

class file_appender : public io_writer {
private:
    phyctx pc_;
    directory *directory_{ nullptr };
    found_file file_;
    simple_buffer buffer_;
    data_chain data_chain_;
    bool truncated_{ false };

public:
    file_appender(phyctx pc, directory *directory, found_file file);

    virtual ~file_appender();

public:
    size_t visited_sectors();

    data_chain_cursor cursor();

    file_size_t position();

public:
    int32_t write_delimiter(size_t delimited_size) {
        uint8_t buffer[4];
        auto size_of_delimiter = varint_encoding_length(delimited_size);
        varint_encode(delimited_size, buffer, sizeof(buffer));
        auto err = write(buffer, size_of_delimiter);
        if (err < 0) {
            return err;
        }
        return size_of_delimiter;
    }

    int32_t seek();

    int32_t write(uint8_t const *data, size_t size) override;

    using io_writer::write;

    int32_t flush();

    int32_t close();

    int32_t index_necessary();

    template<typename tree_type>
    int32_t index_if_necessary(record_number_t record_number) {
        auto err = index_necessary();
        if (err <= 0) {
            return err;
        }

        auto cursor = this->cursor();

        alogf(LogLevels::INFO, "phylum", "indexing position=%" PRIu32 ", psos=%" PRIu32 " cursor=%" PRIu32 " buffer-pos=%zu",
              cursor.position, cursor.position_at_start_of_sector, cursor.sector, buffer_.position());

        tree_type position_index{ data_chain_.pc(), file_.position_index, "pos-idx" };
        err = position_index.add(cursor.position_at_start_of_sector, cursor.sector);
        if (err < 0) {
            return err;
        }

        tree_type record_index{ data_chain_.pc(), file_.record_index, "rec-idx" };
        err = record_index.add(record_number, cursor.position);
        if (err < 0) {
            return err;
        }

        auto position_after = position_index.to_tree_ptr();
        auto record_after = record_index.to_tree_ptr();

        auto position_changed = position_after != file_.position_index;
        auto record_changed = record_after != file_.record_index;

        // Update tree_ptr_t's because they wander.
        if (position_changed || record_changed) {
            auto err = directory_->file_trees(file_.id, position_after, record_after);
            if (err < 0) {
                return err;
            }
        }

        return 1;
    }

    template <typename tree_type>
    int32_t seek_position(uint32_t desired_position) {
        return data_chain_helpers::indexed_seek<tree_type>(data_chain_, file_.position_index, desired_position);
    }

private:
    int32_t make_data_chain();

    bool has_chain() {
        return data_chain_.valid();
    }
};

} // namespace phylum
