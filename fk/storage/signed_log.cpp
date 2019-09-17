#include <fk-data-protocol.h>
#include <phylum/blake2b.h>

#include "storage/signed_log.h"

namespace fk {

FK_DECLARE_LOGGER("srl");

SignedRecordLog::SignedRecordLog(File &file) : file_(file) {
}

bool SignedRecordLog::seek_end() {
    return file_.seek(LastRecord);
}

tl::expected<uint32_t, Error> SignedRecordLog::seek_record(SignedRecordKind kind) {
    // NOTE: We don't have an easy way of seeking in reverse, yet. So we're
    // gonna start at the beginning, for now.
    if (!file_.seek(0)) {
        return tl::unexpected<Error>(Error::IO);
    }

    // TODO: Timeout of some kind?
    RecordReference found;
    while (true) {
        fk_data_SignedRecord sr = fk_data_SignedRecord_init_default;
        auto nread = file_.read(&sr, fk_data_SignedRecord_fields);
        if (nread == 0) {
            break;
        }
        if (sr.kind == (uint32_t)kind) {
            found = file_.reference();
        }
    }

    if (!found.valid()) {
        return tl::unexpected<Error>(Error::IO);
    }

    if (!file_.seek(found)) {
        return tl::unexpected<Error>(Error::IO);
    }

    return file_.record();
}

tl::expected<uint32_t, Error> SignedRecordLog::append_always(SignedRecordKind kind, void const *record, pb_msgdesc_t const *fields, Pool &pool) {
    size_t size = 0;
    auto buffer = pool.encode(fields, record, &size);

    uint8_t hash[Hash::Length];
    BLAKE2b b2b;
    b2b.reset(Hash::Length);
    b2b.update(buffer, size);
    b2b.finalize(&hash, Hash::Length);

    pb_data_t data_ref = {
        .length = size,
        .buffer = buffer,
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

    loginfo("writing");

    if (!file_.write(&sr, fk_data_SignedRecord_fields)) {
        return tl::unexpected<Error>(Error::IO);
    }

    loginfo("appended");

    return sr.record;
}

tl::expected<uint32_t, Error> SignedRecordLog::append_immutable(SignedRecordKind kind, void const *record, pb_msgdesc_t const *fields, Pool &pool) {
    if (seek_record(kind)) {
        // TODO This could be better, for example we could have a custom nanopb
        // ostream that calculates the hash.
        size_t size = 0;
        auto buffer = pool.encode(fields, record, &size);
        FK_ASSERT(buffer != nullptr);

        uint8_t new_hash[Hash::Length];
        BLAKE2b b2b;
        b2b.reset(Hash::Length);
        b2b.update(buffer, size);
        b2b.finalize(&new_hash, Hash::Length);

        fk_data_SignedRecord sr = fk_data_SignedRecord_init_default;
        sr.hash.funcs.decode = pb_decode_data;
        sr.hash.arg = (void *)&pool;
        auto nread = file_.read(&sr, fk_data_SignedRecord_fields);
        if (nread == 0) {
            return tl::unexpected<Error>(Error::IO);
        }

        // Compare the hashes here, ensuring they're the same length.
        auto hash_ref = (pb_data_t *)sr.hash.arg;
        FK_ASSERT(hash_ref->length == Hash::Length);
        if (memcmp(new_hash, hash_ref->buffer, Hash::Length) == 0) {
            loginfo("identical record");
            if (!file_.seek(LastRecord)) {
                return tl::unexpected<Error>(Error::IO);
            }
            return sr.record;
        }
    }
    else {
        loginfo("no record, appending");
    }

    if (!seek_end()) {
        logwarn("creating new file");
    }

    return append_always(kind, record, fields, pool);
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
