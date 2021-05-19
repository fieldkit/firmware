#include "storage/phylum_data_file.h"

namespace fk {

FK_DECLARE_LOGGER("phylum");

using namespace phylum;

static uint8_t get_attribute_for_record_type(RecordType type) {
    switch (type) {
    case RecordType::Modules: return PHYLUM_DRIVER_FILE_ATTR_CONFIG_MODULES;
    case RecordType::Schedule: return PHYLUM_DRIVER_FILE_ATTR_CONFIG_SCHEDULE;
    case RecordType::State: return PHYLUM_DRIVER_FILE_ATTR_CONFIG_STATE;
    case RecordType::Data: return PHYLUM_DRIVER_FILE_ATTR_CONFIG_DATA;
    case RecordType::Location: return PHYLUM_DRIVER_FILE_ATTR_RECORD_LOCATION;
    default:
        break;
    }
    logerror("unexpected record type: %d", type);
    assert(0);
    return 0;
}

PhylumDataFile::PhylumDataFile(Phylum &phylum, Pool &pool) : phylum_(phylum), pool_(pool) {
}

PhylumDataFile::DataFileAttributes PhylumDataFile::attributes() {
    return PhylumDataFile::DataFileAttributes{
        .first_record = 0,
        .nrecords = 0,
        .record_number = 0,
        .size = 0,
    };
}

int32_t PhylumDataFile::initialize_config(Pool &pool) {
    if (attributes_ == nullptr) {
        attributes_ = (open_file_attribute *)pool.malloc(sizeof(open_file_attribute) * PHYLUM_DRIVER_FILE_ATTR_NUMBER);

        auto i = 0;
        attributes_[i++] = open_file_attribute{ PHYLUM_DRIVER_FILE_ATTR_RECORDS,          sizeof(records_attribute_t),  0x00 };
        attributes_[i++] = open_file_attribute{ PHYLUM_DRIVER_FILE_ATTR_RECORD_LOCATION,  sizeof(uint32_t),             0xff };
        attributes_[i++] = open_file_attribute{ PHYLUM_DRIVER_FILE_ATTR_RECORD_UPLOADED,  sizeof(uint32_t),             0xff };
        attributes_[i++] = open_file_attribute{ PHYLUM_DRIVER_FILE_ATTR_CONFIG_MODULES,   sizeof(file_attribute_t),     0xff };
        attributes_[i++] = open_file_attribute{ PHYLUM_DRIVER_FILE_ATTR_CONFIG_SCHEDULE,  sizeof(file_attribute_t),     0xff };
        attributes_[i++] = open_file_attribute{ PHYLUM_DRIVER_FILE_ATTR_CONFIG_STATE,     sizeof(file_attribute_t),     0xff };
        attributes_[i++] = open_file_attribute{ PHYLUM_DRIVER_FILE_ATTR_CONFIG_DATA,      sizeof(file_attribute_t),     0xff };

        assert(i == PHYLUM_DRIVER_FILE_ATTR_NUMBER);

        file_cfg_ = {};
        file_cfg_.attributes = attributes_;
        file_cfg_.nattrs = PHYLUM_DRIVER_FILE_ATTR_NUMBER;

        for (auto i = 0u; i < file_cfg_.nattrs; ++i) {
            auto &attr = attributes_[i];
            attr.ptr = pool.malloc(attr.size);
        }
    }

    for (auto i = 0u; i < file_cfg_.nattrs; ++i) {
        auto &attr = attributes_[i];
        memset(attr.ptr, attr.default_value, attr.size);
    }

    return 0;
}

int32_t PhylumDataFile::open(const char *name, Pool &pool) {
    assert(initialize_config(pool) == 0);

    auto err = dir_.find(name, file_cfg_);
    if (err < 0) {
        return err;
    }

    if (err == 0) {
        return -1;
    }

    name_ = name;

    return 0;
}

int32_t PhylumDataFile::create(const char *name, Pool &pool) {
    assert(name_ == nullptr);
    assert(initialize_config(pool) == 0);

    auto err = dir_.touch_indexed<index_tree_type>(name, file_cfg_);
    if (err < 0) {
        return err;
    }

    name_ = name;

    return 0;
}

class PhylumWriter : public Writer {
private:
    phylum::io_writer *target_;

public:
    PhylumWriter(phylum::io_writer *target) : target_(target) {
    }

public:
    int32_t write(uint8_t const *buffer, size_t size) override {
        return target_->write(buffer, size);
    }

};

class PhylumReader : public Reader {
private:
    phylum::io_reader *target_;

public:
    PhylumReader(phylum::io_reader *target) : target_(target) {
    }

public:
    int32_t read(uint8_t *buffer, size_t size) override {
        return target_->read(buffer, size);
    }

};

class PhylumAttributes {
private:
    open_file_config cfg_;

public:
    PhylumAttributes(open_file_config cfg): cfg_(cfg) {
    }

public:
    template<typename T>
    T *get(uint8_t type) {
        auto &attr = cfg_.attributes[phylum_file_attr_type_to_index(type)];
        attr.dirty = true;
        return (T *)attr.ptr;
    }
};

int32_t PhylumDataFile::append_always(RecordType type, pb_msgdesc_t const *fields, void const *record, Pool &pool) {
    assert(name_ != nullptr);

    loginfo("append-always");

    auto err = dir_.find(name_, file_cfg_);
    if (err < 0) {
        logerror("append-always: find");
        return err;
    }

    if (err == 0) {
        return -1;
    }

    PhylumAttributes attributes{ file_cfg_ };
    auto records = attributes.get<records_attribute_t>(PHYLUM_DRIVER_FILE_ATTR_RECORDS);
    auto data_record = attributes.get<file_attribute_t>(get_attribute_for_record_type(type));

    phylum::file_appender opened{ pc(), &dir_, dir_.open() };
    err = opened.seek();
    if (err < 0) {
        logerror("append-always: seek");
        return err;
    }

    auto record_position = opened.position();

    err = opened.index_if_necessary<index_tree_type>(records->first + records->nrecords);
    if (err < 0) {
        logerror("append-always: index");
        return err;
    }

    blake2b_writer hash_writer{ &opened };

    phylum::io_writer *pwriter = &hash_writer;

    constexpr size_t CobsLookaheadBufferSize = 256;
    write_buffer lookahead{ (uint8_t *)pool.malloc(CobsLookaheadBufferSize), CobsLookaheadBufferSize };
    phylum::cobs_writer cobs{ pwriter, lookahead };
    if (false) {
        cobs.return_bytes_wrote(false);
        pwriter = &cobs;
    }

    PhylumWriter writer{ pwriter };
    auto ostream = pb_ostream_from_writable(&writer);
    if (!pb_encode_delimited(&ostream, fields, record)) {
        logerror("append-always: encode");
        return -1;
    }

    auto bytes_written = ostream.bytes_written;
    auto record_number = records->nrecords;

    hash_writer.finalize(data_record->hash, sizeof(data_record->hash));
    data_record->record = record_number;
    data_record->position = record_position;

    records->nrecords++;

    err = opened.close();
    if (err < 0) {
        logerror("append-always: close");
        return err;
    }

    auto file_size = opened.position();

    auto hash_hex = bytes_to_hex_string_pool(data_record->hash, sizeof(data_record->hash), pool);
    loginfo("wrote record R-%" PRIu32 " position=%zu bytes=%zu total=%zu hash=%s",
            record_number, record_position, bytes_written, file_size, hash_hex);

    return bytes_written;
}

int32_t PhylumDataFile::append_immutable(RecordType type, pb_msgdesc_t const *fields, void const *record, Pool &pool) {
    assert(name_ != nullptr);

    loginfo("append-immutable");

    PhylumAttributes attributes{ file_cfg_ };
    auto file = attributes.get<file_attribute_t>(get_attribute_for_record_type(type));

    phylum::noop_writer noop;
    phylum::blake2b_writer hash_writer{ &noop };
    PhylumWriter writer{ &hash_writer };
    auto ostream = pb_ostream_from_writable(&writer);
    if (!pb_encode_delimited(&ostream, fields, record)) {
        logerror("append-always: encode");
        return -1;
    }

    uint8_t hash[phylum::HashSize];
    hash_writer.finalize(hash, sizeof(hash));

    auto previous_hex = bytes_to_hex_string_pool(file->hash, sizeof(file->hash), pool);
    loginfo("old hash=%s", previous_hex);

    auto hash_hex = bytes_to_hex_string_pool(hash, sizeof(hash), pool);
    loginfo("new hash=%s", hash_hex);

    assert(sizeof(hash) == sizeof(file->hash));
    if (memcmp(file->hash, hash, sizeof(hash)) == 0) {
        return 0;
    }

    auto err = append_always(type, fields, record, pool);
    if (err < 0) {
        return err;
    }

    return err;
}

int32_t PhylumDataFile::seek_record_type(RecordType type, Pool &pool) {
    assert(name_ != nullptr);

    loginfo("seek record-type=%d", type);

    PhylumAttributes attributes{ file_cfg_ };
    auto file = attributes.get<file_attribute_t>(get_attribute_for_record_type(type));
    if (file->position == UINT32_MAX) {
        return -1;
    }

    auto err = seek_position(file->position, pool);
    if (err < 0) {
        return err;
    }

    return err;
}

int32_t PhylumDataFile::seek_record(record_number_t record, Pool &pool) {
    assert(name_ != nullptr);

    loginfo("seek record=%d", record);

    reader_ = new (pool) phylum::file_reader{ pc(), &dir_, dir_.open() };
    auto err = reader_->seek_record<index_tree_type>(record);
    if (err < 0) {
        return err;
    }

    return err;
}

int32_t PhylumDataFile::seek_position(file_size_t position, Pool &pool) {
    assert(name_ != nullptr);

    loginfo("seek position=%d", position);

    reader_ = new (pool) phylum::file_reader{ pc(), &dir_, dir_.open() };
    auto err = reader_->seek_position<index_tree_type>(position);
    if (err < 0) {
        return err;
    }

    return err;
}

int32_t PhylumDataFile::read(uint8_t *data, size_t size, Pool &pool) {
    assert(reader_ != nullptr);

    auto err = reader_->read(data, size);
    if (err < 0) {
        return err;
    }

    return err;
}

int32_t PhylumDataFile::read(pb_msgdesc_t const *fields, void *record, Pool &pool) {
    assert(reader_ != nullptr);

    auto position = reader_->position();

    phylum::io_reader *preader = reader_;

    PhylumReader reader{ preader };
    auto istream = pb_istream_from_readable(&reader);
    if (!pb_decode_delimited(&istream, fields, record)) {
        logerror("read: decode");
        return -1;
    }

    return reader_->position() - position;
}

int32_t PhylumDataFile::close() {
    // I'm not super happy about this, but this is required to free
    // the buffer held by the file_reader, since it's dtor will never
    // be called. I'd much rather fix that behavior.
    // TODO Call destructor for pool allocations.
    if (reader_ != nullptr) {
        reader_->close();
        reader_ = nullptr;
    }
    return 0;
}

}
