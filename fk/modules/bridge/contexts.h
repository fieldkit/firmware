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
    GpsState const *gps_;
    TwoWireWrapper *module_bus_{ nullptr };

public:
    ScanningContext(ModMux *mm, GpsState const *gps, TwoWireWrapper &module_bus, Pool &pool);
    friend class ModuleContext;

public:
    ModuleContext module(ModulePosition position, Pool &pool);
    ReadingsContext readings(ModulePosition position, ModuleReadingsCollection &readings, Pool &pool);

public:
    GpsState const *gps();

};

class ModuleContext {
private:
    ModMux *mm_;
    TwoWireWrapper *module_bus_{ nullptr };
    ModulePosition position_{ ModMux::NoModuleSelected };

public:
    ModuleContext(ScanningContext &from, ModulePosition position, Pool &pool);

public:
    bool open();
    TwoWireWrapper &module_bus();
    bool power_cycle();
    uint32_t now() const;

};

class ReadingsContext : public ModuleContext {
private:
    ModuleReadingsCollection &readings_;

public:
    ReadingsContext(ScanningContext &from, ModulePosition position, ModuleReadingsCollection &readings, Pool &pool);

public:
    ModuleReadingsCollection &readings() {
        return readings_;
    }

};

}
