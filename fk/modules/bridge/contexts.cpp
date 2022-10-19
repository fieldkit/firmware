#include "modules/bridge/contexts.h"
#include "hal/clock.h"

namespace fk {

ScanningContext::ScanningContext(ModMux *mm, GpsState const *gps, TwoWireWrapper &module_bus, Pool &pool)
    : mm_(mm), gps_(gps), module_bus_(&module_bus), power_state_(ModulePowerState::Unknown) {
}

ScanningContext::ScanningContext(ModMux *mm, GpsState const *gps, TwoWireWrapper &module_bus, ModulePowerState power_state, Pool &pool)
    : mm_(mm), gps_(gps), module_bus_(&module_bus), power_state_(power_state) {
}

GpsState const *ScanningContext::gps() {
    return gps_;
}

ModuleContext ScanningContext::open_module(ModulePosition position, Pool &pool) {
    return ModuleContext{ *this, position, pool };
}

ReadingsContext ScanningContext::open_readings(ModulePosition position, Pool &pool) {
    return ReadingsContext{ *this, position, power_state_, pool };
}

ModuleContext::ModuleContext(ScanningContext &from, ModulePosition position, Pool &pool)
    : mm_(from.mm_), module_bus_(from.module_bus_), position_(position) {
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

ReadingsContext::ReadingsContext(ScanningContext &from, ModulePosition position, ModulePowerState power_state, Pool &pool)
    : ModuleContext(from, position, pool), power_state_(power_state) {
}

} // namespace fk
