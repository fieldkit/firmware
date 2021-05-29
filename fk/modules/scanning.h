#pragma once

#include "common.h"
#include "collections.h"
#include "config.h"
#include "hal/modmux.h"
#include "pool.h"
#include "modules/shared/modules.h"

namespace fk {

struct FoundModule {
    ModulePosition position;
    ModuleHeader header;

    bool physical() const {
        return position != ModulePosition::Virtual;
    }
};

using FoundModuleCollection = collection<FoundModule>;

class ScanningListener {
public:
    virtual int32_t scanned_module(ModulePosition const position, ModuleHeader const &header, Pool *pool) = 0;

};

class NoopScanningListener : public ScanningListener {
public:
    int32_t scanned_module(ModulePosition const position, ModuleHeader const &header, Pool *pool) override {
        return 0;
    }

};

class ModuleScanning {
private:
    ModMux *mm_;

public:
    explicit ModuleScanning(ModMux *mm);

public:
    virtual int32_t scan(ScanningListener *listener, Pool &pool);
    virtual bool provision(ModulePosition position, ModuleHeader &header);
    virtual bool configure(ModulePosition position, uint8_t const *buffer, size_t size);
    virtual bool erase(ModulePosition position);

public:
    // Deprecated
    virtual tl::expected<FoundModuleCollection, Error> scan(Pool &pool);

private:
    bool available();
    bool try_scan_single_module(ScanningListener *listener, ModulePosition position, Pool &pool);
    // bool try_read_configuration(ModulePosition position, Pool &pool);

};

class StaticModuleScanning : public ModuleScanning {
private:
    FoundModuleCollection &found_;

public:
    explicit StaticModuleScanning(FoundModuleCollection &found) : ModuleScanning(nullptr), found_(found) {
    }

public:
    tl::expected<FoundModuleCollection, Error> scan(Pool &pool) override {
        return FoundModuleCollection(found_);
    }

};

} // namespace fk
