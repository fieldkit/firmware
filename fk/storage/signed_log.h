#pragma once

#include <fk-data-protocol.h>

#include "file.h"
#include "protobuf.h"
#include "containers.h"

namespace fk {

enum class SignedRecordKind : uint8_t {
    None = fk_data_SignedRecordKind_SIGNED_RECORD_KIND_NONE,
    Modules = fk_data_SignedRecordKind_SIGNED_RECORD_KIND_MODULES,
    Schedule = fk_data_SignedRecordKind_SIGNED_RECORD_KIND_SCHEDULE,
    State = fk_data_SignedRecordKind_SIGNED_RECORD_KIND_STATE,
    Other = fk_data_SignedRecordKind_SIGNED_RECORD_KIND_OTHER,
};

class SignedRecordLog {
private:
    File &file_;

public:
    SignedRecordLog(File &file);

public:
    tl::expected<uint32_t, Error> seek_record(SignedRecordKind kind);
    tl::expected<uint32_t, Error> append_always(SignedRecordKind kind, void const *record, pb_msgdesc_t const *fields, Pool &pool);
    tl::expected<uint32_t, Error> append_immutable(SignedRecordKind kind, void const *record, pb_msgdesc_t const *fields, Pool &pool);
    bool decode(void *record, pb_msgdesc_t const *fields, Pool &pool);
    bool seek_end();

};


}
