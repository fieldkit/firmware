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

ModuleReadings::ModuleReadings() {
}

ModuleReadings::ModuleReadings(size_t n) : nreadings_(n), readings_(new ModuleReading[n]) {
}

ModuleReadings::~ModuleReadings() {
    if (readings_ != nullptr) {
        delete [] readings_;
        readings_ = nullptr;
    }
}

size_t ModuleReadings::size() const {
    return nreadings_;
}

float ModuleReadings::get(int32_t i) const {
    return readings_[i].value;
}

void ModuleReadings::set(int32_t i, float value) {
    readings_[i].value = value;
}

}
