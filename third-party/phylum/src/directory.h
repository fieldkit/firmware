#pragma once

#include "writer.h"

namespace phylum {

struct open_file_attribute {
    uint8_t type{ 0 };
    uint8_t size{ 0 };
    uint8_t default_value{ 0 };
    void   *ptr{ nullptr };
    bool    dirty{ 0 };

    open_file_attribute() {
    }

    open_file_attribute(uint8_t type, uint8_t size, uint8_t default_value = 0x00) : type(type), size(size), default_value(default_value) {
    }
};

enum class open_file_flags {
    None = 0,
    Truncate = 1,
};

struct open_file_config {
    open_file_flags flags{ open_file_flags::None };
    open_file_attribute *attributes{ nullptr };
    size_t nattrs{ 0 };

    open_file_config() {
    }
};

struct found_file {
    file_id_t id{ UINT32_MAX };
    file_size_t directory_size{ UINT32_MAX };
    file_size_t directory_capacity{ UINT32_MAX };
    head_tail_t chain;
    tree_ptr_t position_index;
    tree_ptr_t record_index;
    sector_position_t record;
    open_file_config cfg;
};

class directory {
public:
    virtual int32_t mount() = 0;

    virtual int32_t format() = 0;

    virtual int32_t touch(const char *name) = 0;

    virtual int32_t unlink(const char *name) = 0;

    virtual int32_t find(const char *name, open_file_config file_cfg) = 0;

    virtual found_file open() = 0;

public:
    virtual int32_t file_attributes(file_id_t id, open_file_attribute *attributes, size_t nattrs) = 0;

    virtual int32_t file_chain(file_id_t id, head_tail_t chain) = 0;

    virtual int32_t file_data(file_id_t id, file_size_t position, uint8_t const *buffer, size_t size) = 0;

    virtual int32_t file_trees(file_id_t id, tree_ptr_t position_index, tree_ptr_t record_index) = 0;

    virtual int32_t read(file_id_t id, io_writer &writer) = 0;

};

}
