#include "storage/data_record.h"
#include "records.h"

namespace fk {

DataRecord::DataRecord() {
    record_ = fk_data_DataRecord_init_default;
}

}
