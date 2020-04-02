#include "modules/bridge/contexts.h"
#include "clock.h"

namespace fk {

ScanningContext::ScanningContext(ModMux *mm, GlobalState const *gs, TwoWireWrapper &module_bus, Pool &pool) : mm_(mm), gs_(gs), module_bus_(&module_bus) {
}

ScanningContext::~ScanningContext() {
}

GlobalState const *ScanningContext::gs() {
    return gs_;
}

ModuleContext ScanningContext::module(int32_t position, Pool &pool) {
    return { *this, position, pool };
}

ModuleContext::ModuleContext(ScanningContext &from, int32_t position, Pool &pool) : mm_(from.mm_), gs_(from.gs_), module_bus_(from.module_bus_), position_(position) {
}

ModuleContext::~ModuleContext() {
}

bool ModuleContext::open() {
    return mm_->choose(position_);
}

GlobalState const *ModuleContext::gs() {
    return gs_;
}

TwoWireWrapper &ModuleContext::module_bus() {
    return *module_bus_;
}

bool ModuleContext::power_cycle() {
    return mm_->power_cycle(position_);
}

uint32_t ModuleContext::now() const {
    return get_clock_now();
}

}
