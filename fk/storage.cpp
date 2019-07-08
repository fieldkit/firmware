#include "storage.h"

namespace fk {

Storage::Storage(DataMemory *memory) : memory_(memory) {
}

bool Storage::begin() {
    return true;
}

bool Storage::clear() {
    // auto g = memory_->geometry();

    return true;
}

bool Storage::append(fk_data_DataRecord *record) {
    size_t size = 0;
    auto fields = fk_data_DataRecord_fields;
    if (!pb_get_encoded_size(&size, fields, record)) {
        return false;
    }

    return true;
}

}
