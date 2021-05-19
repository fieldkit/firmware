#pragma once

#include "storage/phylum.h"
#include "storage/file_ops.h"

namespace fk {

struct index_attribute_t {
    uint32_t record{ (uint32_t)-1 };
    uint32_t position{ (uint32_t)-1 };
    uint8_t hash[32];

    index_attribute_t() {
        memset(hash, 0xff, sizeof(hash));
    }
};

struct records_attribute_t {
    uint32_t first;
    uint32_t nrecords;
};

#define PHYLUM_DRIVER_FILE_ATTR_RECORDS              (0x01)
#define PHYLUM_DRIVER_FILE_ATTR_INDEX_LOCATION       (0x02)
#define PHYLUM_DRIVER_FILE_ATTR_INDEX_UPLOADED       (0x03)
#define PHYLUM_DRIVER_FILE_ATTR_INDEX_MODULES        (0x04)
#define PHYLUM_DRIVER_FILE_ATTR_INDEX_SCHEDULE       (0x05)
#define PHYLUM_DRIVER_FILE_ATTR_INDEX_STATE          (0x06)
#define PHYLUM_DRIVER_FILE_ATTR_INDEX_DATA           (0x07)
#define PHYLUM_DRIVER_FILE_ATTR_NUMBER               (0x07)

static inline uint8_t phylum_file_attr_type_to_index(uint8_t type) {
    return type - 1;
}

enum class RecordType : uint8_t {
    Unknown,
    Modules,
    Schedule,
    State,
    Data,
    Location,
};

using record_number_t = phylum::record_number_t;
using file_size_t = phylum::file_size_t;

class PhylumDataFile {
private:
    static constexpr dhara_sector_t RootDirectorySector = 0;
    Phylum &phylum_;
    Pool &pool_;
    const char *name_{ nullptr };
    phylum::open_file_config file_cfg_{ };
    phylum::open_file_attribute *attributes_{ nullptr };
    directory_type dir_{ pc(), RootDirectorySector };
    phylum::file_reader *reader_{ nullptr };

public:
    PhylumDataFile(Phylum &phylum, Pool &pool);

public:
    int32_t open(const char *name, Pool &pool);
    int32_t create(const char *name, Pool &pool);
    int32_t append_always(RecordType type, pb_msgdesc_t const *fields, void const *record, Pool &pool);
    int32_t append_immutable(RecordType type, pb_msgdesc_t const *fields, void const *record, Pool &pool);

public:
    int32_t seek_record_type(RecordType type, file_size_t &position, Pool &pool);
    int32_t seek_record(record_number_t record, Pool &pool);
    int32_t seek_position(file_size_t position, Pool &pool);
    int32_t read(uint8_t *data, size_t size, Pool &pool);
    int32_t read(pb_msgdesc_t const *fields, void *record, Pool &pool);
    int32_t close();

public:
    struct DataFileAttributes {
        record_number_t first_record;
        record_number_t nrecords;
        record_number_t record_number;
        file_size_t size;
    };

    DataFileAttributes attributes();

    bool is_open() const {
        return name_ != nullptr;
    }

private:
    int32_t initialize_config(Pool &pool);

    phylum::phyctx pc() {
        return phylum_.pc();
    }

};

}
