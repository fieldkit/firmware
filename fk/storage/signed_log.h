#pragma once

#include "file.h"
#include "protobuf.h"

namespace fk {

enum class SignedRecordKind : uint8_t {
    None = 0,
    Modules = 1,
    Schedule = 2,
    Other = 255,
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
