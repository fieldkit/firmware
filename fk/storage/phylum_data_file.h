#pragma once

#include "storage/phylum.h"
#include "storage/file_ops.h"

namespace fk {

struct file_attribute_t {
    uint32_t record{ (uint32_t)-1 };
    uint32_t position{ (uint32_t)-1 };
    uint8_t hash[32];

    file_attribute_t() {
        memset(hash, 0xff, sizeof(hash));
    }
};

struct records_attribute_t {
    uint32_t first;
    uint32_t nrecords;
};

#define PHYLUM_DRIVER_FILE_ATTR_RECORDS              (0x01)
#define PHYLUM_DRIVER_FILE_ATTR_RECORD_GPS           (0x02)
#define PHYLUM_DRIVER_FILE_ATTR_RECORD_UPLOADED      (0x03)
#define PHYLUM_DRIVER_FILE_ATTR_CONFIG_MODULES       (0x04)
#define PHYLUM_DRIVER_FILE_ATTR_CONFIG_SCHEDULE      (0x05)
#define PHYLUM_DRIVER_FILE_ATTR_CONFIG_STATE         (0x06)
#define PHYLUM_DRIVER_FILE_ATTR_CONFIG_DATA          (0x07)
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
};

using record_number_t = phylum::record_number_t;

class PhylumDataFile {
private:
    Phylum &phylum_;
    const char *name_{ nullptr };
    phylum::open_file_config file_cfg_{ };
    phylum::open_file_attribute attributes_[PHYLUM_DRIVER_FILE_ATTR_NUMBER];

public:
    PhylumDataFile(Phylum &phylum, const char *name);

public:
    int32_t open(Pool &pool);
    int32_t create(Pool &pool);
    int32_t append_always(RecordType type, pb_msgdesc_t const *fields, void const *record, Pool &pool);
    int32_t append_immutable(RecordType type, pb_msgdesc_t const *fields, void const *record, Pool &pool);
    int32_t seek_record_type(RecordType type, Pool &pool);
    int32_t seek_record(uint32_t record, Pool &pool);
    int32_t seek_position(uint32_t position, Pool &pool);
    int32_t read(RecordType type, Pool &pool);

private:
    int32_t initialize_config(Pool &pool);

    phylum::phyctx pc() {
        return phylum_.pc();
    }

};

};
