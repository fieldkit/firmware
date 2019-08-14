#pragma once

#include "file.h"
#include "protobuf.h"

namespace fk {

enum class SignedRecordKind {
    None,
    Modules,
};

class SignedRecordLog {
private:
    File &file_;

public:
    SignedRecordLog(File &file);

public:
    bool seek_end();
    bool seek_record(SignedRecordKind kind);
    bool append(SignedRecordKind kind, void const *record, pb_msgdesc_t const *fields, Pool &pool);
    bool decode(void *record, pb_msgdesc_t const *fields, Pool &pool);
    bool append_immutable(SignedRecordKind kind, void const *record, pb_msgdesc_t const *fields, Pool &pool);

};


}
