#pragma once

#include "hal/board.h"
#include "hal/modmux.h"
#include "state.h"
#include "modules/bridge/data.h"

namespace fk {

class ModuleContext;
class ReadingsContext;

class ScanningContext {
private:
    ModMux *mm_;
    GlobalState const *gs_;
    TwoWireWrapper *module_bus_{ nullptr };

public:
    ScanningContext(ModMux *mm, GlobalState const *gs, TwoWireWrapper &module_bus, Pool &pool);
    friend class ModuleContext;

public:
    ModuleContext module(int32_t position, Pool &pool);
    ReadingsContext readings(int32_t position, ModuleReadingsCollection &readings, Pool &pool);

public:
    GlobalState const *gs();

};

class ModuleContext {
private:
    ModMux *mm_;
    GlobalState const *gs_;
    TwoWireWrapper *module_bus_{ nullptr };
    int32_t position_{ -1 };

public:
    ModuleContext(ScanningContext &from, int32_t position, Pool &pool);

public:
    bool open();
    GlobalState const *gs();
    TwoWireWrapper &module_bus();
    bool power_cycle();
    uint32_t now() const;

};

class ReadingsContext : public ModuleContext {
private:
    ModuleReadingsCollection &readings_;

public:
    ReadingsContext(ScanningContext &from, int32_t position, ModuleReadingsCollection &readings, Pool &pool);

public:
    ModuleReadingsCollection &readings() {
        return readings_;
    }

};

}
