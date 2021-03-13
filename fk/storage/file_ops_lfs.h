#pragma once

#include "storage/file_ops.h"
#include "storage/file_map.h"
#include "storage/partitioned_reader.h"

namespace fk {

namespace lfs {

class MetaOps : public fk::MetaOps {
private:
    LfsDriver &lfs_;
    FileMap map_;

public:
    explicit MetaOps(LfsDriver &lfs);

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
    LfsDriver &lfs_;
    FileMap map_;

public:
    explicit DataOps(LfsDriver &lfs);

public:
    tl::expected<uint32_t, Error> write_readings(GlobalState *gs, fk_data_DataRecord *record, Pool &pool) override;
    tl::expected<FileAttributes, Error> atttributes() override;
    bool read_fixed_record(DataRecord &record, Pool &pool) override;
};

class FileReader : public fk::FileReader {
private:
    LfsDriver &lfs_;
    FileNumber file_number_;
    Pool &pool_;
    FileMap map_;
    PartitionedReader partitioned_{ &lfs_, &map_, pool_ };

public:
    explicit FileReader(LfsDriver &lfs, FileNumber file_number, Pool &pool);

public:
    SizeInfo get_size(BlockNumber first_block, BlockNumber last_block, Pool &pool) override;

    bool decode_signed(void *record, pb_msgdesc_t const *fields, Pool &pool) override;

public:
    bool seek_record(RecordNumber record, Pool &pool) override;
    int32_t read(uint8_t *record, size_t size) override;
    int32_t read(void *record, pb_msgdesc_t const *fields) override;
};

} // namespace lfs

} // namespace fk
