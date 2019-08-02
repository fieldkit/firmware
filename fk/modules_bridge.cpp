#include "modules_bridge.h"

namespace fk {

ModuleContext::ModuleContext() {
}

ModuleContext::ModuleContext(ModuleContext &from, int32_t module) : module_(module) {
}

ModuleContext::~ModuleContext() {
}

ModuleContext ModuleContext::module(int32_t module) {
    return { *this, module };
}

}
