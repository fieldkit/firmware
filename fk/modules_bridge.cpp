#include "modules_bridge.h"

namespace fk {

ModuleContext::ModuleContext() {
}

ModuleContext::ModuleContext(GlobalState const *gs, TwoWireWrapper &module_bus) : gs_(gs), module_bus_(&module_bus) {
}

ModuleContext::ModuleContext(ModuleContext &from, int32_t module) : gs_(from.gs_), module_bus_(from.module_bus_), module_(module) {
}

ModuleContext::~ModuleContext() {
}

ModuleContext ModuleContext::module(int32_t module) {
    return { *this, module };
}

}
