#pragma once

#include <fk-data-protocol.h>

#include "file.h"
#include "protobuf.h"

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
    bool seek_end();
    bool seek_record(SignedRecordKind kind);
    bool append_always(SignedRecordKind kind, void const *record, pb_msgdesc_t const *fields, Pool &pool);
    bool append_immutable(SignedRecordKind kind, void const *record, pb_msgdesc_t const *fields, Pool &pool);
    bool decode(void *record, pb_msgdesc_t const *fields, Pool &pool);

};


}
