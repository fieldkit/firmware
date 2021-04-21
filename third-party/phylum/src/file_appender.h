#pragma once

#include "data_chain.h"
#include "simple_buffer.h"
#include "directory.h"

namespace phylum {

class file_appender {
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

    int32_t write(char const *str) {
        return write((uint8_t *)str, strlen(str));
    }

    int32_t write(char const *str, size_t size) {
        return write((uint8_t *)str, size);
    }

    int32_t write(uint8_t const *data, size_t size);

    uint32_t u32(uint8_t type);

    void u32(uint8_t type, uint32_t value);

    int32_t flush();

    int32_t close();

    int32_t index_if_necessary(std::function<int32_t(data_chain_cursor)> fn);

    template<typename tree_type>
    int32_t index_if_necessary(record_number_t record_number) {
        int32_t err;

        err = index_if_necessary([&](data_chain_cursor cursor) {
            alogf(LogLevels::INFO, "phylum", "indexing position=%d, psos=%d cursor=%d", cursor.position, cursor.position_at_start_of_sector, cursor.sector);

            tree_type position_index{ data_chain_.pc(), file_.position_index, "posidx" };
            err = position_index.add(cursor.position_at_start_of_sector, cursor.sector);
            if (err < 0) {
                return err;
            }

            tree_type record_index{ data_chain_.pc(), file_.record_index, "recidx" };
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

            return 0;
        });

        return err;
    }

private:
    int32_t make_data_chain();

    bool has_chain() {
        return data_chain_.valid();
    }
};

} // namespace phylum
