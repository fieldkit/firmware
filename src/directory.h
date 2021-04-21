#pragma once

namespace phylum {

struct open_file_attribute {
    uint8_t type{ 0 };
    uint8_t size{ 0 };
    void   *ptr{ nullptr };
    bool    dirty{ 0 };
};

enum class open_file_flags {
    None = 0,
    Truncate = 1,
};

struct open_file_config {
    open_file_flags flags{ open_file_flags::None };
    open_file_attribute *attributes{ nullptr };
    size_t nattrs{ 0 };
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

    virtual int32_t read(file_id_t id, std::function<int32_t(read_buffer)> fn) = 0;

};

}
