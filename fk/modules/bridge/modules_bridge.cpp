#include "modules_bridge.h"

namespace fk {

ModuleContext::ModuleContext() {
}

ModuleContext::ModuleContext(ModMux *mm, GlobalState const *gs, TwoWireWrapper &module_bus) : mm_(mm), gs_(gs), module_bus_(&module_bus) {
}

ModuleContext::ModuleContext(ModuleContext &from, int32_t position) : mm_(from.mm_), gs_(from.gs_), module_bus_(from.module_bus_), position_(position) {
}

ModuleContext::~ModuleContext() {
}

bool ModuleContext::open() {
    return mm_->choose(position_);
}

ModuleContext ModuleContext::module(int32_t position) {
    return { *this, position };
}

}
