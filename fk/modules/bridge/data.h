#pragma once

#include "common.h"
#include "collections.h"
#include "modules/bridge/meta.h"
#include "modules/shared/uuid.h"

#include "modules/scanning.h"

namespace fk {

/**
 * Describes the power needs for a Module.
 */
enum class ModulePower {
    ReadingsOnly,
    Always,
};

/**
** Value for module ordering and ranking.
 */
using ModuleOrder = uint16_t;

/**
 * Default module order, right smack in the middle.
 */
constexpr ModuleOrder DefaultModuleOrder = UINT16_MAX / 2;

/**
 * Configuration information a module can provide to the OS.
 */
typedef struct ModuleConfiguration {
    /**
     * Localization key to use to display the module's name.
     */
    const char *display_name_key{ "modules.unknown" };

    /**
     * Power needs for the module.
     */
    ModulePower power{ ModulePower::ReadingsOnly };

    /**
     * How often the module needs to be serviced.
     */
    uint32_t service_interval{ 0 };

    /**
     * Preferred module servicing order. This is mostly a hint to the
     * core when to service this module. Most modules will have an
     * average order that effectively means they'll be serviced in
     * physical module order.
     */
    ModuleOrder service_order{ DefaultModuleOrder };

    /**
     * Shortest interval between readings.
     */
    uint32_t minimum_readings_interval{ 0 };

    /**
     * Constructor
     */
    ModuleConfiguration() {
    }

    /**
     * Constructor
     */
    ModuleConfiguration(const char *display_name_key)
        : display_name_key(display_name_key) {
    }

    /**
     * Constructor
     */
    ModuleConfiguration(const char *display_name_key, ModuleOrder order)
        : display_name_key(display_name_key), service_order(order) {
    }

    /**
     * Constructor
     */
    ModuleConfiguration(const char *display_name_key, ModulePower power, uint32_t service_interval)
        : display_name_key(display_name_key), power(power), service_interval(service_interval) {
    }

    /**
     * Constructor
     */
    ModuleConfiguration(const char *display_name_key, ModulePower power, uint32_t service_interval, ModuleOrder order)
        : display_name_key(display_name_key), power(power), service_interval(service_interval), service_order(order) {
    }
} ModuleConfiguration;

class ModuleReadings {
public:
    virtual size_t size() const = 0;
    virtual void set(int32_t i, float value) = 0;
    virtual float get(int32_t i) const = 0;
    virtual ModuleReadings *clone(Pool &pool) const = 0;
};

/**
 * Metadata for a particular sensor.
 */
typedef struct SensorMetadata {
    const char *name;
    const char *unitOfMeasure;
    uint32_t flags;
} SensorMetadata;

/**
 * Information on all the sensors attached to a module.
 */
typedef struct ModuleSensors {
    size_t nsensors;
    SensorMetadata const *sensors;
} ModuleSensors;

struct ModuleMetaAndReadings {
    uint16_t position;
    fk_uuid_t *id;
    ModuleMetadata const *meta;
    ModuleSensors const *sensors;
    ModuleReadings const *readings;
    ModuleConfiguration configuration;
};

using ModuleReadingsCollection = collection<ModuleMetaAndReadings>;

enum class ModuleStatus {
    Unknown = 0,
    Empty,
    Found,
    Ok,
    Warning,
    Fatal
};

struct ConstructedModule {
    FoundModule found;
    ModuleMetadata const *meta;
    Module *module;
    ModuleStatus status;
    ModuleConfiguration configuration;
};

using ConstructedModulesCollection = collection<ConstructedModule>;

}
