#include "storage/phylum_data_file.h"

namespace fk {

FK_DECLARE_LOGGER("phylum");

using namespace phylum;

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

static uint8_t get_attribute_for_record_type(RecordType type) {
    switch (type) {
    case RecordType::Modules: return PHYLUM_DRIVER_FILE_ATTR_INDEX_MODULES;
    case RecordType::Schedule: return PHYLUM_DRIVER_FILE_ATTR_INDEX_SCHEDULE;
    case RecordType::State: return PHYLUM_DRIVER_FILE_ATTR_INDEX_STATE;
    case RecordType::Data: return PHYLUM_DRIVER_FILE_ATTR_INDEX_DATA;
    case RecordType::Location: return PHYLUM_DRIVER_FILE_ATTR_INDEX_LOCATION;
    default:
        break;
    }
    logerror("unexpected record type: %d", type);
    assert(0);
    return 0;
}

PhylumDataFile::PhylumDataFile(Phylum &phylum, Pool &pool) : phylum_(phylum), pool_(pool) {
}

int32_t PhylumDataFile::initialize_config(Pool &pool) {
    if (attributes_ == nullptr) {
        attributes_ = (open_file_attribute *)pool.malloc(sizeof(open_file_attribute) * PHYLUM_DRIVER_FILE_ATTR_NUMBER);

        auto i = 0;
        attributes_[i++] = open_file_attribute{ PHYLUM_DRIVER_FILE_ATTR_RECORDS,          sizeof(records_attribute_t),  0x00 };
        attributes_[i++] = open_file_attribute{ PHYLUM_DRIVER_FILE_ATTR_INDEX_LOCATION,   sizeof(index_attribute_t),    0xff };
        attributes_[i++] = open_file_attribute{ PHYLUM_DRIVER_FILE_ATTR_INDEX_UPLOADED,   sizeof(index_attribute_t),    0xff };
        attributes_[i++] = open_file_attribute{ PHYLUM_DRIVER_FILE_ATTR_INDEX_MODULES,    sizeof(index_attribute_t),    0xff };
        attributes_[i++] = open_file_attribute{ PHYLUM_DRIVER_FILE_ATTR_INDEX_SCHEDULE,   sizeof(index_attribute_t),    0xff };
        attributes_[i++] = open_file_attribute{ PHYLUM_DRIVER_FILE_ATTR_INDEX_STATE,      sizeof(index_attribute_t),    0xff };
        attributes_[i++] = open_file_attribute{ PHYLUM_DRIVER_FILE_ATTR_INDEX_DATA,       sizeof(index_attribute_t),    0xff };

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

PhylumDataFile::DataFileAttributes PhylumDataFile::attributes() {
    assert(name_ != nullptr);

    PhylumAttributes attributes{ file_cfg_ };
    auto records = attributes.get<records_attribute_t>(PHYLUM_DRIVER_FILE_ATTR_RECORDS);

    if (size_ == 0 && records->nrecords) {
        auto err = seek_position(UINT32_MAX, pool_);
        if (err < 0) {
            logerror("seeking end for size");
        }
    }

    return PhylumDataFile::DataFileAttributes{
        .first_record = records->first,
        .nrecords = records->nrecords,
        .record_number = records->nrecords,
        .size = size_,
    };
}

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

    logdebug("append-always: attributes");

    auto attribute_type = get_attribute_for_record_type(type);
    auto attribute_index = phylum_file_attr_type_to_index(attribute_type);

    PhylumAttributes attributes{ file_cfg_ };
    auto records = attributes.get<records_attribute_t>(PHYLUM_DRIVER_FILE_ATTR_RECORDS);
    auto index_record = attributes.get<index_attribute_t>(attribute_type);

    logdebug("append-always: opening");

    phylum::file_appender opened{ pc(), &dir_, dir_.open() };

    logdebug("append-always: seeking");
    err = opened.seek_position<index_tree_type>(UINT32_MAX);
    if (err < 0) {
        logerror("append-always: seeking");
        return err;
    }

    auto record_position = opened.position();

    logdebug("append-always: index-if-necessary");

    err = opened.index_if_necessary<index_tree_type>(records->first + records->nrecords);
    if (err < 0) {
        logerror("append-always: index");
        return err;
    }

    logdebug("append-always: writers");

    blake2b_writer hash_writer{ &opened };

    phylum::io_writer *pwriter = &hash_writer;

    constexpr size_t CobsLookaheadBufferSize = 256;
    write_buffer lookahead{ (uint8_t *)pool.malloc(CobsLookaheadBufferSize), CobsLookaheadBufferSize };
    phylum::cobs_writer cobs{ pwriter, lookahead };
    if (false) {
        cobs.return_bytes_wrote(false);
        pwriter = &cobs;
    }

    logdebug("append-always: encoding");

    PhylumWriter writer{ pwriter };
    auto ostream = pb_ostream_from_writable(&writer);
    if (!pb_encode_delimited(&ostream, fields, record)) {
        logerror("append-always: encode");
        return -1;
    }

    logdebug("append-always: finalizing");

    auto bytes_written = ostream.bytes_written;
    auto record_number = records->nrecords;

    logdebug("append-always: record-type=%d attribute-type=%d index=%d number=%" PRIu32 " position=%" PRIu32,
             type, attribute_type, attribute_index, record_number, record_position);

    hash_writer.finalize(index_record->hash, sizeof(index_record->hash));
    index_record->record = record_number;
    index_record->position = record_position;
    index_record->nrecords++;

    records->nrecords++;

    logdebug("append-always: closing");

    err = opened.close();
    if (err < 0) {
        logerror("append-always: close");
        return err;
    }

    auto file_size = opened.position();

    size_ = file_size;

    auto hash_hex = bytes_to_hex_string_pool(index_record->hash, sizeof(index_record->hash), pool);
    loginfo("wrote record R-%" PRIu32 " position=%zu bytes=%zu total=%zu hash=%s",
            record_number, record_position, bytes_written, file_size, hash_hex);

    return bytes_written;
}

int32_t PhylumDataFile::append_immutable(RecordType type, pb_msgdesc_t const *fields, void const *record, Pool &pool) {
    assert(name_ != nullptr);

    loginfo("append-immutable");

    PhylumAttributes attributes{ file_cfg_ };
    auto index_attribute = attributes.get<index_attribute_t>(get_attribute_for_record_type(type));

    phylum::noop_writer noop;
    phylum::blake2b_writer hash_writer{ &noop };
    PhylumWriter writer{ &hash_writer };
    auto ostream = pb_ostream_from_writable(&writer);
    if (!pb_encode_delimited(&ostream, fields, record)) {
        logerror("append-immutable: encode");
        return -1;
    }

    uint8_t hash[phylum::HashSize];
    hash_writer.finalize(hash, sizeof(hash));

    auto previous_hex = bytes_to_hex_string_pool(index_attribute->hash, sizeof(index_attribute->hash), pool);
    loginfo("old hash=%s", previous_hex);

    auto hash_hex = bytes_to_hex_string_pool(hash, sizeof(hash), pool);
    loginfo("new hash=%s", hash_hex);

    assert(sizeof(hash) == sizeof(index_attribute->hash));
    if (memcmp(index_attribute->hash, hash, sizeof(hash)) == 0) {
        return 0;
    }

    auto err = append_always(type, fields, record, pool);
    if (err < 0) {
        return err;
    }

    return err;
}

int32_t PhylumDataFile::seek_record_type(RecordType type, file_size_t &position, Pool &pool) {
    assert(name_ != nullptr);

    auto attribute_type = get_attribute_for_record_type(type);
    auto attribute_index = phylum_file_attr_type_to_index(attribute_type);

    loginfo("seek record-type=%d attribute-type=%d index=%d", type, attribute_type, attribute_index);

    PhylumAttributes attributes{ file_cfg_ };
    auto index_attribute = attributes.get<index_attribute_t>(attribute_type);

    position = index_attribute->position;

    if (position == UINT32_MAX) {
        return 0;
    }

    auto err = seek_position(position, pool);
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

    return reader_->position();
}

int32_t PhylumDataFile::seek_position(file_size_t position, Pool &pool) {
    assert(name_ != nullptr);

    loginfo("seek position=%d", position);

    reader_ = new (pool) phylum::file_reader{ pc(), &dir_, dir_.open() };
    auto err = reader_->seek_position<index_tree_type>(position);
    if (err < 0) {
        return err;
    }

    if (position == UINT32_MAX) {
        size_ = reader_->position();
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

    auto position_after = reader_->position();

    if (position_after > size_) {
        size_ = position_after;
    }

    return position_after - position;
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
