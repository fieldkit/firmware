#include "storage/data_record.h"
#include "records.h"

namespace fk {

fk_data_DataRecord &DataRecord::for_decoding(Pool &pool) {
    return record_ = fk_data_record_decoding_new(pool);
}

fk_data_DataRecord const &DataRecord::record() {
    return record_;
}

} // namespace fk
