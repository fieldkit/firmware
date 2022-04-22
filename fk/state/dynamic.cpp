#include "exchange.h"
#include "state/dynamic.h"

namespace fk {

namespace state {

DynamicState::DynamicState() {
    pool_ = create_standard_pool_inside("modules-state");
    attached_ = new (pool_) state::AttachedModules{ *pool_ };
}

DynamicState::DynamicState(DynamicState &&rhs) : pool_(exchange(rhs.pool_, nullptr)), attached_(exchange(rhs.attached_, nullptr)) {
}

DynamicState::~DynamicState() {
    if (pool_ != nullptr) {
        delete pool_;
        pool_ = nullptr;
        attached_ = nullptr;
    }
}

DynamicState &DynamicState::operator=(DynamicState &&rhs) {
    alogf(LogLevels::INFO, "dynamic", "attaching");

    if (this != &rhs) {
        if (pool_ != nullptr) {
            delete pool_;
            pool_ = nullptr;
            attached_ = nullptr;
        }
        pool_ = rhs.pool_;
        rhs.pool_ = nullptr;
        attached_ = rhs.attached_;
        rhs.attached_ = nullptr;
    }
    return *this;
}

} // namespace state

} // namespace fk
