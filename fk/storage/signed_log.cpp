#include <fk-data-protocol.h>
#include <phylum/blake2b.h>

#include "storage/signed_log.h"
#include "storage/storage.h"
#include "clock.h"

namespace fk {

FK_DECLARE_LOGGER("srl");

SignedRecordLog::SignedRecordLog(File &file) : file_(file) {
}

bool SignedRecordLog::seek_end() {
    return file_.seek(LastRecord);
}

tl::expected<uint32_t, Error> SignedRecordLog::seek_record(SignedRecordKind kind) {
    if (!file_.seek_end()) {
        return tl::unexpected<Error>(Error::IO);
    }

    logdebug("[" PRADDRESS "] seek_record %" PRIu32 " position=%" PRIu32 " record=#%" PRIu32, file_.tail(), (int32_t)kind, file_.position(), file_.record());

    if (file_.record() == 1 || file_.position() == 0) {
        return tl::unexpected<Error>(Error::EoF);
    }

    if (!file_.rewind()) {
        logerror("[" PRADDRESS "] rewind failed", file_.tail());
        return tl::unexpected<Error>(Error::IO);
    }

    RecordReference found;
    while (true) {
        logtrace("[" PRADDRESS "] finding %" PRIu32 " position = %" PRIu32, file_.tail(), (int32_t)kind, file_.position());

        fk_data_SignedRecord sr = fk_data_SignedRecord_init_default;
        auto nread = file_.read(&sr, fk_data_SignedRecord_fields);
        if (nread == 0) {
            logdebug("[" PRADDRESS "] end of file", file_.tail());
            break;
        }

        if (!file_.rewind()) {
            logerror("[" PRADDRESS "] rewind failed", file_.tail());
            return tl::unexpected<Error>(Error::IO);
        }

        if (sr.kind == (uint32_t)kind) {
            found = file_.reference();
            break;
        }

        if (!file_.rewind()) {
            logerror("[" PRADDRESS "] rewind failed", file_.tail());
            return tl::unexpected<Error>(Error::IO);
        }
    }

    if (!found.valid()) {
        return tl::unexpected<Error>(Error::IO);
    }

    logdebug("[" PRADDRESS "] found record #%" PRIu32 " position = %" PRIu32,
             file_.tail(), file_.record(), file_.position());

    return file_.record();
}

tl::expected<AppendedRecord, Error> SignedRecordLog::append_always(SignedRecordKind kind, void const *record, pb_msgdesc_t const *fields, Pool &pool) {
    auto encoded = pool.encode(fields, record);

    uint8_t hash[Hash::Length];
    BLAKE2b b2b;
    b2b.reset(Hash::Length);
    b2b.update(encoded->buffer, encoded->size);
    b2b.finalize(&hash, Hash::Length);

    pb_data_t data_ref = {
        .length = encoded->size,
        .buffer = encoded->buffer,
    };

    pb_data_t hash_ref = {
        .length = Hash::Length,
        .buffer = hash,
    };

    fk_data_SignedRecord sr = fk_data_SignedRecord_init_default;
    sr.kind = (fk_data_SignedRecordKind)kind;
    sr.data.funcs.encode = pb_encode_data;
    sr.data.arg = (void *)&data_ref;
    sr.hash.funcs.encode = pb_encode_data;
    sr.hash.arg = (void *)&hash_ref;
    sr.record = file_.record();
    sr.time = get_clock_now();

    auto record_size = file_.write(&sr, fk_data_SignedRecord_fields);
    if (record_size == 0) {
        return tl::unexpected<Error>(Error::IO);
    }

    return AppendedRecord{ (uint32_t)sr.record, (uint32_t)record_size };
}

tl::expected<AppendedRecord, Error> SignedRecordLog::append_immutable(SignedRecordKind kind, void const *record, pb_msgdesc_t const *fields, Pool &pool) {
    auto sought = seek_record(kind);
    if (sought) {
        // TODO This could be better, for example we could have a custom nanopb
        // ostream that calculates the hash.
        auto encoded = pool.encode(fields, record);
        FK_ASSERT(encoded != nullptr);

        uint8_t new_hash[Hash::Length];
        BLAKE2b b2b;
        b2b.reset(Hash::Length);
        b2b.update(encoded->buffer, encoded->size);
        b2b.finalize(&new_hash, Hash::Length);

        fk_data_SignedRecord sr = fk_data_SignedRecord_init_default;
        sr.hash.funcs.decode = pb_decode_data;
        sr.hash.arg = (void *)&pool;
        sr.data.funcs.decode = pb_decode_data;
        sr.data.arg = (void *)&pool;

        auto nread = file_.read(&sr, fk_data_SignedRecord_fields);
        if (nread == 0) {
            return tl::unexpected<Error>(Error::IO);
        }

        // Compare the hashes here, ensuring they're the same length.
        auto hash_ref = (pb_data_t *)sr.hash.arg;
        FK_ASSERT(hash_ref->length == Hash::Length);
        if (memcmp(new_hash, hash_ref->buffer, Hash::Length) == 0) {
            logdebug("[" PRADDRESS "] identical record", file_.tail());
            if (!file_.seek(LastRecord)) {
                return tl::unexpected<Error>(Error::IO);
            }
            return AppendedRecord{ (uint32_t)sr.record, 0u };
        }
        else {
            if (log_is_debug()) {
                auto record_data = (pb_data_t *)sr.data.arg;
                fk_dump_memory("saved ", (uint8_t *)record_data->buffer, record_data->length);
                fk_dump_memory("wrote ", (uint8_t *)encoded->buffer, encoded->size);
            }
        }
    }
    else {
        loginfo("no record, appending");
    }

    if (!seek_end()) {
        logwarn("creating new file");
    }

    #if defined(FK_VERBOSE_SIGNED_LOG_WRITES)
    ScopedLogLevelChange enable_trace{ LogLevels::TRACE };
    #endif

    auto rv = append_always(kind, record, fields, pool);

    return rv;
}

bool SignedRecordLog::decode(void *record, pb_msgdesc_t const *fields, Pool &pool) {
    fk_data_SignedRecord sr = fk_data_SignedRecord_init_default;
    sr.data.funcs.decode = pb_decode_data;
    sr.data.arg = (void *)&pool;
    sr.data.funcs.decode = pb_decode_data;
    sr.hash.arg = (void *)&pool;

    auto nread = file_.read(&sr, fk_data_SignedRecord_fields);
    if (nread == 0) {
        return false;
    }

    auto data_ref = (pb_data_t *)sr.data.arg;
    auto stream = pb_istream_from_buffer((pb_byte_t *)data_ref->buffer, data_ref->length);
    if (!pb_decode_delimited(&stream, fields, record)) {
        return false;
    }

    return true;
}


}
