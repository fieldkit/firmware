#include "storage/phylum_data_file.h"

namespace fk {

FK_DECLARE_LOGGER("phylum");

using namespace phylum;

static const char *get_attribute_name(uint8_t type) {
    switch (type) {
    case PHYLUM_DRIVER_FILE_ATTR_RECORDS:
        return "records";
    case PHYLUM_DRIVER_FILE_ATTR_INDEX_LOCATION:
        return "location";
    case PHYLUM_DRIVER_FILE_ATTR_INDEX_UPLOADED:
        return "uploaded";
    case PHYLUM_DRIVER_FILE_ATTR_INDEX_MODULES:
        return "modules";
    case PHYLUM_DRIVER_FILE_ATTR_INDEX_SCHEDULE:
        return "schedule";
    case PHYLUM_DRIVER_FILE_ATTR_INDEX_STATE:
        return "state";
    case PHYLUM_DRIVER_FILE_ATTR_INDEX_DATA:
        return "data";
    default:
        return "UNKNOWN";
    }
}

class PhylumAttributes {
private:
    open_file_config &cfg_;
    Pool &pool_;

public:
    PhylumAttributes(open_file_config &cfg, Pool &pool): cfg_(cfg), pool_(pool) {
    }

public:
    template<typename T>
    T *get(uint8_t type) {
        auto &attr = cfg_.attributes[phylum_file_attr_type_to_index(type)];
        attr.dirty = true;
        return (T *)attr.ptr;
    }

    void initialize() {
        if (cfg_.attributes != nullptr) {
            return;
        }

        auto attributes =
            (open_file_attribute *)pool_.malloc(sizeof(open_file_attribute) * PHYLUM_DRIVER_FILE_ATTR_NUMBER);

        auto i = 0;
        attributes[i++] = open_file_attribute{ PHYLUM_DRIVER_FILE_ATTR_RECORDS, sizeof(records_attribute_t), 0x00 };
        attributes[i++] = open_file_attribute{ PHYLUM_DRIVER_FILE_ATTR_INDEX_LOCATION, sizeof(index_attribute_t), 0xff };
        attributes[i++] = open_file_attribute{ PHYLUM_DRIVER_FILE_ATTR_INDEX_UPLOADED, sizeof(index_attribute_t), 0xff };
        attributes[i++] = open_file_attribute{ PHYLUM_DRIVER_FILE_ATTR_INDEX_MODULES, sizeof(index_attribute_t), 0xff };
        attributes[i++] = open_file_attribute{ PHYLUM_DRIVER_FILE_ATTR_INDEX_SCHEDULE, sizeof(index_attribute_t), 0xff };
        attributes[i++] = open_file_attribute{ PHYLUM_DRIVER_FILE_ATTR_INDEX_STATE, sizeof(index_attribute_t), 0xff };
        attributes[i++] = open_file_attribute{ PHYLUM_DRIVER_FILE_ATTR_INDEX_DATA, sizeof(index_attribute_t), 0xff };

        assert(i == PHYLUM_DRIVER_FILE_ATTR_NUMBER);

        cfg_ = {};
        cfg_.attributes = attributes;
        cfg_.nattrs = PHYLUM_DRIVER_FILE_ATTR_NUMBER;

        for (auto index = 0u; index < cfg_.nattrs; ++index) {
            auto &attr = attributes[index];
            attr.ptr = pool_.malloc(attr.size);

            if (index == 0) {
                *((records_attribute_t *)attr.ptr) = records_attribute_t{};
            } else {
                *((index_attribute_t *)attr.ptr) = index_attribute_t{};
            }
        }
    }

    void debug() {
        char hash_hex[Hash::Length * 2 + 1];
        for (auto index = 0u; index < cfg_.nattrs; ++index) {
            auto &attr = cfg_.attributes[index];
            auto name = get_attribute_name(attr.type);
            if (index == 0) {
                auto value = (records_attribute_t *)attr.ptr;
                loginfo("attribute[%d] %-8s first-record=%" PRIu32 " nrecords=%" PRIu32 "", index, name, value->first, value->nrecords);
            } else {
                auto value = (index_attribute_t *)attr.ptr;
                if (value->nrecords == 0) {
                    loginfo("attribute[%d] %-8s nrecords=%" PRIu32 "", index, name, value->nrecords);
                }
                else {
                    bytes_to_hex_string(hash_hex, sizeof(hash_hex), value->hash, sizeof(value->hash));
                    loginfo("attribute[%d] %-8s nrecords=%" PRIu32 " record=%" PRIu32 " position=%" PRIu32 " hash=%s",
                            index, name, value->nrecords, value->record, value->position, hash_hex);
                }
            }
        }
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
    PhylumAttributes attributes{ file_cfg_, pool_ };
    attributes.initialize();

    return 0;
}

int32_t PhylumDataFile::open(const char *name, Pool &pool) {
    assert(initialize_config(pool) == 0);

    logged_task lt{ "df-open" };

    auto err = dir_.find(name, file_cfg_);
    if (err < 0) {
        return err;
    }

    if (err == 0) {
        return -1;
    }

    name_ = name;

    PhylumAttributes attributes{ file_cfg_, pool_ };
    attributes.debug();

    return 0;
}

int32_t PhylumDataFile::create(const char *name, Pool &pool) {
    assert(name_ == nullptr);
    assert(initialize_config(pool) == 0);

    logged_task lt{ "df-create" };

    auto err = dir_.touch_indexed<index_tree_type>(name, file_cfg_);
    if (err < 0) {
        return err;
    }

    name_ = name;

    return 0;
}

PhylumDataFile::DataFileAttributes PhylumDataFile::attributes() {
    assert(name_ != nullptr);

    logged_task lt{ "df-attrs" };

    PhylumAttributes attributes{ file_cfg_, pool_ };

    auto records = attributes.get<records_attribute_t>(PHYLUM_DRIVER_FILE_ATTR_RECORDS);
    auto data_index = attributes.get<index_attribute_t>(PHYLUM_DRIVER_FILE_ATTR_INDEX_DATA);

    // If size is 0 and we have records we know we haven't calculated
    // the size.
    if (size_ == 0 && records->nrecords > 0) {
        auto err = seek_position(UINT32_MAX, pool_);
        if (err < 0) {
            logerror("seeking end for size");
        }
    }

    loginfo("attributes first=%" PRIu32 " nrecords=%" PRIu32 " nreadings=%" PRIu32 " size=%" PRIu32,
            records->first, records->nrecords, data_index->nrecords, size_);

    return PhylumDataFile::DataFileAttributes{
        .first_record = records->first,
        .nrecords = records->nrecords,
        .nreadings = data_index->nrecords,
        .size = size_,
    };
}

PhylumDataFile::appended_t PhylumDataFile::append_always(RecordType type, pb_msgdesc_t const *fields, void const *record, Pool &pool) {
    assert(name_ != nullptr);

    logged_task lt{ "df-always" };

    loginfo("append-always");

    auto err = dir_.find(name_, file_cfg_);
    if (err < 0) {
        logerror("append-always: find");
        return appended_t{ err };
    }

    if (err == 0) {
        return appended_t{ -1 };
    }

    logdebug("append-always: attributes");

    auto attribute_type = get_attribute_for_record_type(type);
    auto attribute_index = phylum_file_attr_type_to_index(attribute_type);

    PhylumAttributes attributes{ file_cfg_, pool_ };
    auto records = attributes.get<records_attribute_t>(PHYLUM_DRIVER_FILE_ATTR_RECORDS);
    auto index_record = attributes.get<index_attribute_t>(attribute_type);

    logdebug("append-always: opening");

    phylum::file_appender opened{ pc(), &dir_, dir_.open() };

    logdebug("append-always: seeking");
    err = opened.seek_position<index_tree_type>(UINT32_MAX);
    if (err < 0) {
        logerror("append-always: seeking");
        return appended_t{ err };
    }

    auto record_position = opened.position();

    logdebug("append-always: index-if-necessary");

    err = opened.index_if_necessary<index_tree_type>(records->first + records->nrecords);
    if (err < 0) {
        logerror("append-always: index");
        return appended_t{ err };
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
        return appended_t{ -1 };
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
        return appended_t{ err };
    }

    auto file_size = opened.position();

    size_ = file_size;

    auto hash_hex = bytes_to_hex_string_pool(index_record->hash, sizeof(index_record->hash), pool);
    loginfo("wrote record R-%" PRIu32 " position=%zu bytes=%zu total=%zu hash=%s",
            record_number, record_position, bytes_written, file_size, hash_hex);

    attributes.debug();

    return appended_t{ (int32_t)bytes_written, record_number };
}

PhylumDataFile::appended_t PhylumDataFile::append_immutable(RecordType type, pb_msgdesc_t const *fields, void const *record, Pool &pool) {
    assert(name_ != nullptr);

    logged_task lt{ "df-idemp" };

    loginfo("append-immutable");

    PhylumAttributes attributes{ file_cfg_, pool_ };
    auto index_attribute = attributes.get<index_attribute_t>(get_attribute_for_record_type(type));

    phylum::noop_writer noop;
    phylum::blake2b_writer hash_writer{ &noop };
    PhylumWriter writer{ &hash_writer };
    auto ostream = pb_ostream_from_writable(&writer);
    if (!pb_encode_delimited(&ostream, fields, record)) {
        logerror("append-immutable: encode");
        return appended_t{ -1 };
    }

    uint8_t hash[phylum::HashSize];
    hash_writer.finalize(hash, sizeof(hash));

    auto previous_hex = bytes_to_hex_string_pool(index_attribute->hash, sizeof(index_attribute->hash), pool);
    loginfo("old hash=%s", previous_hex);

    auto hash_hex = bytes_to_hex_string_pool(hash, sizeof(hash), pool);
    loginfo("new hash=%s", hash_hex);

    assert(sizeof(hash) == sizeof(index_attribute->hash));
    if (memcmp(index_attribute->hash, hash, sizeof(hash)) == 0) {
        return appended_t{ 0, index_attribute->record };
    }

    auto appended = append_always(type, fields, record, pool);
    if (appended.bytes < 0) {
        return appended;
    }

    return appended;
}

int32_t PhylumDataFile::seek_record_type(RecordType type, file_size_t &position, Pool &pool) {
    assert(name_ != nullptr);

    logged_task lt{ "df-seek-rec-type" };

    auto attribute_type = get_attribute_for_record_type(type);
    auto attribute_index = phylum_file_attr_type_to_index(attribute_type);

    loginfo("seek record-type=%d attribute-type=%d index=%d", type, attribute_type, attribute_index);

    PhylumAttributes attributes{ file_cfg_, pool_ };
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

    logged_task lt{ "df-seek-rec" };

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

    logged_task lt{ "df-seek-pos" };

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

    logged_task lt{ "df-read" };

    auto err = reader_->read(data, size);
    if (err < 0) {
        return err;
    }

    return err;
}

int32_t PhylumDataFile::read(pb_msgdesc_t const *fields, void *record, Pool &pool) {
    assert(reader_ != nullptr);

    logged_task lt{ "df-read" };

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
        logged_task lt{ "df-close" };
        reader_->close();
        reader_ = nullptr;
    }
    return 0;
}

}
