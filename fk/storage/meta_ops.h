#pragma once

#include <loading.h>

#include "storage/signed_log.h"
#include "storage/meta_record.h"
#include "storage/data_record.h"
#include "modules/module_factory.h"
#include "state.h"

namespace fk {

class Storage;

struct FileAttributes {
    uint32_t size;
    uint32_t records;
};

class MetaOps {
private:
    Storage &storage_;

public:
    explicit MetaOps(Storage &storage);

public:
    tl::expected<uint32_t, Error> write_state(GlobalState *gs, Pool &pool);
    tl::expected<uint32_t, Error> write_state(GlobalState *gs, fkb_header_t const *fkb_header, Pool &pool);
    tl::expected<uint32_t, Error> write_modules(GlobalState *gs, fkb_header_t const *fkb_header, ConstructedModulesCollection &modules, ModuleReadingsCollection &readings, Pool &pool);

    tl::expected<FileAttributes, Error> atttributes();

    bool read_record(SignedRecordKind kind, MetaRecord &record, Pool &pool);

private:
    tl::expected<uint32_t, Error> write_kind(GlobalState *gs, SignedRecordKind kind, MetaRecord &record, Pool &pool);

};

class DataOps {
private:
    Storage &storage_;

public:
    explicit DataOps(Storage &storage);

public:
    tl::expected<uint32_t, Error> write_readings(GlobalState *gs, fk_data_DataRecord *record, Pool &pool);
    tl::expected<FileAttributes, Error> atttributes();

public:
    bool read_fixed_record(DataRecord &record, Pool &pool);

};

class FileReader  {
private:
    Storage &storage_;
    FileNumber file_number_;
    Pool &pool_;
    File file_;

public:
    explicit FileReader(Storage &storage, FileNumber file_number, Pool &pool);

public:
    struct SizeInfo {
        StorageSize size;
        BlockNumber last_block;
    };

    SizeInfo get_size(BlockNumber first_block, BlockNumber last_block, Pool &pool);

    bool decode_signed(void *record, pb_msgdesc_t const *fields, Pool &pool);

public:
    bool seek_record(RecordNumber record);
    int32_t read(uint8_t *record, size_t size);
    int32_t read(void *record, pb_msgdesc_t const *fields);

};

}
