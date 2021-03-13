#pragma once

#include "storage/file_ops.h"

namespace fk {

namespace darwin {

class MetaOps : public fk::MetaOps {
private:
    Storage &storage_;

public:
    explicit MetaOps(Storage &storage);

public:
    tl::expected<uint32_t, Error> write_state(GlobalState *gs, Pool &pool) override;
    tl::expected<uint32_t, Error> write_state(GlobalState *gs, fkb_header_t const *fkb_header, Pool &pool) override;
    tl::expected<uint32_t, Error> write_modules(GlobalState *gs, fkb_header_t const *fkb_header,
                                                ConstructedModulesCollection &modules,
                                                ModuleReadingsCollection &readings, Pool &pool) override;

    tl::expected<FileAttributes, Error> atttributes() override;

    bool read_record(SignedRecordKind kind, MetaRecord &record, Pool &pool) override;

private:
    tl::expected<uint32_t, Error> write_kind(GlobalState *gs, SignedRecordKind kind, MetaRecord &record, Pool &pool);
};

class DataOps : public fk::DataOps {
private:
    Storage &storage_;

public:
    explicit DataOps(Storage &storage);

public:
    tl::expected<uint32_t, Error> write_readings(GlobalState *gs, fk_data_DataRecord *record, Pool &pool) override;
    tl::expected<FileAttributes, Error> atttributes() override;
    bool read_fixed_record(DataRecord &record, Pool &pool) override;
};

class FileReader : public fk::FileReader {
private:
    Storage &storage_;
    FileNumber file_number_;
    Pool &pool_;
    File file_;

public:
    explicit FileReader(Storage &storage, FileNumber file_number, Pool &pool);

public:
    SizeInfo get_size(BlockNumber first_block, BlockNumber last_block, Pool &pool) override;

    bool decode_signed(void *record, pb_msgdesc_t const *fields, Pool &pool) override;

public:
    bool seek_record(RecordNumber record, Pool &pool) override;
    int32_t read(uint8_t *record, size_t size) override;
    int32_t read(void *record, pb_msgdesc_t const *fields) override;
};

} // namespace darwin

} // namespace fk
