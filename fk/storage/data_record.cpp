#include "storage/data_record.h"
#include "records.h"

namespace fk {

fk_data_DataRecord &DataRecord::for_decoding(Pool &pool) {
    if (record_ == nullptr) {
        record_ = pool.malloc<fk_data_DataRecord>();
    }
    fk_data_record_decoding_new(record_, pool);
    return *record_;
}

fk_data_DataRecord const &DataRecord::record() {
    FK_ASSERT(record_ != nullptr);
    return *record_;
}

} // namespace fk
