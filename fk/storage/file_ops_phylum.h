#pragma once

#include "storage/file_ops.h"
#include "storage/phylum.h"

namespace fk {

namespace phylum_ops {

class MetaOps : public fk::MetaOps {
private:
    Storage &storage_;

public:
    explicit MetaOps(Storage &storage);

public:
    tl::expected<uint32_t, Error> write_record(SignedRecordKind kind, fk_data_DataRecord *record, Pool &pool) override;
    tl::expected<FileAttributes, Error> attributes(Pool &pool) override;
    bool read_record(SignedRecordKind kind, MetaRecord &record, Pool &pool) override;

private:
    tl::expected<uint32_t, Error> write_kind(RecordType record_type, fk_data_DataRecord *record, Pool &pool);
};

class DataOps : public fk::DataOps {
private:
    Storage &storage_;

public:
    explicit DataOps(Storage &storage);

public:
    tl::expected<uint32_t, Error> write_readings(fk_data_DataRecord *record, Pool &pool) override;
    tl::expected<FileAttributes, Error> attributes(Pool &pool) override;
    bool read_fixed_record(DataRecord &record, Pool &pool) override;
    bool touch(Pool &pool);
};

class FileReader : public fk::FileReader {
private:
    Storage &storage_;
    FileNumber file_number_;
    PhylumDataFile pdf_;
    Pool &pool_;

public:
    explicit FileReader(Storage &storage, FileNumber file_number, Pool &pool);

public:
    tl::expected<SizeInfo, Error> get_size(BlockNumber first_block, BlockNumber last_block, Pool &pool) override;

    bool decode_signed(void *record, pb_msgdesc_t const *fields, Pool &pool) override;

public:
    bool seek_record(RecordNumber record, Pool &pool) override;
    int32_t read(uint8_t *record, size_t size) override;
    int32_t read(void *record, pb_msgdesc_t const *fields) override;
    int32_t get_file_size(size_t &file_size) override;
    EncodedMessage *read_signed_record_bytes(SignedRecordKind kind, Pool &pool) override;

private:
    bool open_if_necessary();
};

} // namespace phylum_ops

} // namespace fk
