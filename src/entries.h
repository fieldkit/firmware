#pragma once

#include <cinttypes>
#include <cstddef>

namespace phylum {

constexpr size_t MaximumNameLength = 64;

typedef uint32_t file_id_t;
typedef uint32_t file_size_t;
typedef uint16_t file_flags_t;
typedef uint32_t record_number_t;
typedef uint16_t sector_offset_t;
typedef uint32_t dhara_sector_t;

constexpr dhara_sector_t InvalidSector = (dhara_sector_t)-1;

#define PHY_PACKED __attribute__((__packed__))

struct PHY_PACKED head_tail_t {
    dhara_sector_t head{ InvalidSector };
    dhara_sector_t tail{ InvalidSector };

    head_tail_t() {
    }

    head_tail_t(dhara_sector_t head, dhara_sector_t tail) : head(head), tail(tail) {
    }

    bool valid() const {
        return head != InvalidSector;
    }

    bool operator!=(const head_tail_t &other) const {
        return head != other.head || tail != other.tail;
    }

    bool operator==(const head_tail_t &other) const {
        return head == other.head && tail == other.tail;
    }
};

struct PHY_PACKED tree_ptr_t {
    dhara_sector_t root{ InvalidSector };
    dhara_sector_t tail{ InvalidSector };

    tree_ptr_t() {
    }

    explicit tree_ptr_t(dhara_sector_t first) : root(first), tail(first) {
    }

    explicit tree_ptr_t(dhara_sector_t root, dhara_sector_t tail) : root(root), tail(tail) {
    }

    bool valid() const {
        return root != InvalidSector;
    }

    bool operator!=(const tree_ptr_t &other) const {
        return root != other.root || tail != other.tail;
    }

    bool operator==(const tree_ptr_t &other) const {
        return root == other.root && tail == other.tail;
    }
};

enum entry_type : uint8_t {
    None = 0,
    SuperBlock = 1,
    DataSector = 2,
    DirectorySector = 3,
    FreeChainSector = 4,
    TreeSector = 5,
    FileEntry = 6,
    FsFileEntry = 7,
    FsDirectoryEntry = 8,
    FileData = 9,
    TreeNode = 10,
    FileAttribute = 11,
    FreeSectors = 12,
};

struct PHY_PACKED entry_t {
    entry_type type{ entry_type::None };

    entry_t(entry_type type) : type(type) {
    }
};

enum class sector_flags : uint8_t {
    None = 0,
    Tail = 1,
};

struct PHY_PACKED sector_chain_header_t : entry_t {
    dhara_sector_t pp{ (dhara_sector_t)InvalidSector };
    dhara_sector_t np{ (dhara_sector_t)InvalidSector };
    sector_flags flags{ sector_flags::None };

    sector_chain_header_t(entry_type type) : entry_t(type) {
    }

    sector_chain_header_t(entry_type type, dhara_sector_t pp, dhara_sector_t np) : entry_t(type), pp(pp), np(np) {
    }

    sector_chain_header_t(entry_type type, dhara_sector_t pp, dhara_sector_t np, sector_flags flags) : entry_t(type), pp(pp), np(np), flags(flags) {
    }
};

struct PHY_PACKED super_block_t : sector_chain_header_t {
    char magic[8];
    uint32_t version{ 1 };
    tree_ptr_t directory_tree{ };
    head_tail_t free_chain{ };

    super_block_t() : sector_chain_header_t(entry_type::SuperBlock) {
        bzero(magic, sizeof(magic));
        strncpy(magic, "phylum", sizeof(magic));
    }
};

struct PHY_PACKED directory_chain_header_t : sector_chain_header_t {
    directory_chain_header_t() : sector_chain_header_t(entry_type::DirectorySector) {
    }

    directory_chain_header_t(dhara_sector_t pp, dhara_sector_t np) : sector_chain_header_t(entry_type::DirectorySector, pp, np) {
    }

    directory_chain_header_t(dhara_sector_t pp, dhara_sector_t np, sector_flags flags) : sector_chain_header_t(entry_type::DirectorySector, pp, np, flags) {
    }
};

struct PHY_PACKED free_chain_header_t : sector_chain_header_t {
    free_chain_header_t() : sector_chain_header_t(entry_type::FreeChainSector) {
    }

    free_chain_header_t(dhara_sector_t pp, dhara_sector_t np) : sector_chain_header_t(entry_type::FreeChainSector, pp, np) {
    }
};

struct PHY_PACKED data_chain_header_t : sector_chain_header_t {
    uint16_t bytes{ 0 };

    data_chain_header_t() : sector_chain_header_t(entry_type::DataSector) {
    }

    data_chain_header_t(uint16_t bytes) : sector_chain_header_t(entry_type::DataSector), bytes(bytes) {
    }

    data_chain_header_t(uint16_t bytes, dhara_sector_t pp, dhara_sector_t np) : sector_chain_header_t(entry_type::DataSector, pp, np), bytes(bytes) {
    }

    data_chain_header_t(uint16_t bytes, dhara_sector_t pp, dhara_sector_t np, sector_flags flags) : sector_chain_header_t(entry_type::DataSector, pp, np, flags), bytes(bytes) {
    }
};

inline uint32_t make_file_id(const char *path) {
    return crc32_checksum(path);
}

enum class FsDirTreeFlags : uint16_t {
    None = 0,
    Deleted = 1 << 0,
};

struct PHY_PACKED dirtree_entry_t : entry_t {
    char name[MaximumNameLength];
    file_flags_t flags;
    uint32_t reserved[3] = { 0, 0, 0 };

    dirtree_entry_t(entry_type type, const char *full_name, uint16_t flags) : entry_t(type), flags(flags) {
        bzero(name, sizeof(name));
        strncpy(name, full_name, sizeof(name));
    }
};

struct PHY_PACKED dirtree_dir_t : dirtree_entry_t {
    dhara_sector_t attributes{ InvalidSector };
    dhara_sector_t children{ InvalidSector };

    dirtree_dir_t(const char *name, uint16_t flags = 0)
        : dirtree_entry_t(entry_type::FsDirectoryEntry, name, flags) {
    }
};

struct PHY_PACKED dirtree_file_t : dirtree_entry_t {
    file_size_t directory_size{ 0 };
    head_tail_t chain;
    tree_ptr_t attributes{ };
    tree_ptr_t position_index{ };
    tree_ptr_t record_index{ };

    dirtree_file_t(const char *name, uint16_t flags = 0)
        : dirtree_entry_t(entry_type::FsFileEntry, name, flags) {
    }
};

template<size_t Storage>
struct PHY_PACKED dirtree_tree_value_t {
    union PHY_PACKED entry_union {
        dirtree_entry_t e;
        dirtree_dir_t dir;
        dirtree_file_t file;

        entry_union() {
        }
    } u;

    uint8_t data[Storage];

    dirtree_tree_value_t() {
    }
};

struct PHY_PACKED file_entry_t : entry_t {
    file_id_t id;
    file_flags_t flags;
    char name[MaximumNameLength];

    file_entry_t(file_id_t id, const char *full_name, uint16_t flags = 0)
        : entry_t(entry_type::FileEntry), id(id), flags(flags) {
        bzero(name, sizeof(name));
        strncpy(name, full_name, sizeof(name));
    }

    file_entry_t(const char *full_name, uint16_t flags = 0)
        : entry_t(entry_type::FileEntry), id(make_file_id(full_name)), flags(flags) {
        bzero(name, sizeof(name));
        strncpy(name, full_name, sizeof(name));
    }

    file_entry_t(file_id_t id, uint16_t flags = 0)
        : entry_t(entry_type::FileEntry), id(id), flags(flags) {
        bzero(name, sizeof(name));
    }
};

struct PHY_PACKED file_data_t : entry_t {
    file_id_t id{ 0 };
    file_size_t size{ 0 };
    head_tail_t chain;
    tree_ptr_t attributes{ };
    tree_ptr_t position_index{ };
    tree_ptr_t record_index{ };

    file_data_t(file_id_t id, file_size_t size) : entry_t(entry_type::FileData), id(id), size(size) {
    }

    file_data_t(file_id_t id, head_tail_t chain)
        : entry_t(entry_type::FileData), id(id), chain(chain) {
    }
};

struct PHY_PACKED free_sectors_t : entry_t {
    dhara_sector_t head{ InvalidSector };

    free_sectors_t()
        : entry_t(entry_type::FreeSectors) {
    }

    free_sectors_t(dhara_sector_t head)
        : entry_t(entry_type::FreeSectors), head(head) {
    }
};

struct PHY_PACKED file_attribute_t : entry_t {
    file_id_t id{ 0 };
    uint8_t type{ 0 };
    uint8_t size{ 0 };

    file_attribute_t(file_id_t id, uint8_t type, uint8_t size)
        : entry_t(entry_type::FileAttribute), id(id), type(type), size(size) {
    }
};

struct PHY_PACKED node_ptr_t {
    dhara_sector_t sector;
    sector_offset_t position;

    node_ptr_t() : sector(InvalidSector), position(0) {
    }

    node_ptr_t(dhara_sector_t sector, size_t position): sector(sector), position((sector_offset_t)position) {
    }

    node_ptr_t(dhara_sector_t sector, sector_offset_t position): sector(sector), position(position) {
    }

    bool valid() const {
        // NOTE It's also true that position can never be zero for a
        // node_ptr_t, because headers are zero-length.
        return sector != InvalidSector;
    }

    bool operator!=(const node_ptr_t &other) const {
        return sector != other.sector || position != other.position;
    }

    bool operator==(const node_ptr_t &other) const {
        return sector == other.sector && position == other.position;
    }
};

using sector_position_t = node_ptr_t;

using depth_type = uint8_t;
using index_type = int16_t;

enum node_type : uint8_t {
    Leaf,
    Inner,
};

struct PHY_PACKED tree_node_header_t : entry_t {
public:
    depth_type depth{ 0 };
    index_type number_keys{ 0 };
    node_type type{ node_type::Leaf };
    union PHY_PACKED debug_t {
        uint8_t reserved[11];
        dhara_sector_t sector;
    };
    debug_t dbg;

public:
    tree_node_header_t() : entry_t(entry_type::TreeNode) {
    }

    tree_node_header_t(node_type type) : entry_t(entry_type::TreeNode), type(type) {
    }
};

template <typename KEY, typename VALUE, size_t Size>
struct tree_node_t : tree_node_header_t {
public:
    typedef KEY key_type;
    typedef VALUE value_type;

    union data_t {
        VALUE values[Size];
        node_ptr_t children[Size + 1];

        data_t() {
        }
    };

public:
    data_t d;
    KEY keys[Size];

public:
    tree_node_t() : tree_node_header_t() {
        clear();
    }

    tree_node_t(node_type type) : tree_node_header_t(type) {
        clear();
    }

public:
    void clear() {
        depth = 0;
        number_keys = 0;
        for (auto i = 0u; i < Size; ++i) {
            keys[i] = 0;
            new (&d.values[i]) VALUE{};
            d.children[i] = {};
        }
        for (auto &ref : d.children) {
            ref = {};
        }
    }

    bool empty() const {
        return number_keys == 0;
    }
};

} // namespace phylum
