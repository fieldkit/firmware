#include "modules/bridge/contexts.h"
#include "clock.h"

namespace fk {

ScanningContext::ScanningContext(ModMux *mm, GpsState const *gps, TwoWireWrapper &module_bus, Pool &pool) : mm_(mm), gps_(gps), module_bus_(&module_bus) {
}

GpsState const *ScanningContext::gps() {
    return gps_;
}

ModuleContext ScanningContext::module(ModulePosition position, Pool &pool) {
    return { *this, position, pool };
}

ReadingsContext ScanningContext::readings(ModulePosition position, ModuleReadingsCollection &readings, Pool &pool) {
    return { *this, position, readings, pool };
}

ModuleContext::ModuleContext(ScanningContext &from, ModulePosition position, Pool &pool) : mm_(from.mm_), module_bus_(from.module_bus_), position_(position) {
}

bool ModuleContext::open() {
    return mm_->choose(position_);
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

ReadingsContext::ReadingsContext(ScanningContext &from, ModulePosition position, ModuleReadingsCollection &readings, Pool &pool) : ModuleContext(from, position, pool), readings_(readings) {
}

}
