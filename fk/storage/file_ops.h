#pragma once

#include <loading.h>

#include "state.h"
#include "storage/data_record.h"
#include "storage/meta_record.h"
#include "storage/signed_log.h"

namespace fk {

class Storage;

struct FileAttributes {
    uint32_t size;
    uint32_t records;
};

class MetaOps {
public:
    virtual tl::expected<uint32_t, Error> write_state(GlobalState *gs, Pool &pool) = 0;
    virtual tl::expected<uint32_t, Error> write_state(GlobalState *gs, fkb_header_t const *fkb_header, Pool &pool) = 0;
    virtual tl::expected<uint32_t, Error> write_modules(GlobalState *gs, fkb_header_t const *fkb_header,
                                                        ConstructedModulesCollection &modules,
                                                        ModuleReadingsCollection &readings, Pool &pool) = 0;
    virtual tl::expected<FileAttributes, Error> atttributes() = 0;
    virtual bool read_record(SignedRecordKind kind, MetaRecord &record, Pool &pool) = 0;
};

class DataOps {
public:
    virtual tl::expected<uint32_t, Error> write_readings(GlobalState *gs, fk_data_DataRecord *record, Pool &pool) = 0;
    virtual tl::expected<FileAttributes, Error> atttributes() = 0;
    virtual bool read_fixed_record(DataRecord &record, Pool &pool) = 0;
};

class FileReader {
public:
    struct SizeInfo {
        StorageSize size;
        BlockNumber last_block;
    };

    virtual SizeInfo get_size(BlockNumber first_block, BlockNumber last_block, Pool &pool) = 0;

    virtual bool decode_signed(void *record, pb_msgdesc_t const *fields, Pool &pool) = 0;

public:
    virtual bool seek_record(RecordNumber record) = 0;
    virtual int32_t read(uint8_t *record, size_t size) = 0;
    virtual int32_t read(void *record, pb_msgdesc_t const *fields) = 0;
};

} // namespace fk
