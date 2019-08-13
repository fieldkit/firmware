#include "meta.h"

namespace fk {

MetaLog::MetaLog(Storage *storage, GlobalState const *gs) : storage_(storage), gs_(gs) {
    record_ = fk_data_DataRecord_init_default;
}

MetaLog::~MetaLog() {
}

bool MetaLog::open() {
    auto file = storage_->file(0);
    if (file.size() > 0) {
    }

    return true;
}

fk_data_DataRecord &MetaLog::record() {
    return record_;
}

}
