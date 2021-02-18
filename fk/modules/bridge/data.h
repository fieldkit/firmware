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
 * Module order for internal modules.
 */
constexpr ModuleOrder ModuleOrderInternal = DefaultModuleOrder  - 10;

/**
 * Module order for modules that provide calibration data other
 * modules may use.
 */
constexpr ModuleOrder ModuleOrderProvidesCalibration = DefaultModuleOrder  - 1;

/**
 * Configuration information a module can provide to the OS.
 */
typedef struct ModuleConfiguration {
    /**
     * Localization key to use to display the module's name.
     */
    const char *display_name_key{ "modules.unknown" };

    /**
     * Delimiter encoded ModuleConfiguration.
     */
    EncodedMessage *message{ nullptr };

    /**
     * Power needs for the module.
     */
    ModulePower power{ ModulePower::ReadingsOnly };

    /**
     * How long to wait after powering up the module.
     */
    uint32_t wake_delay{ 100 };

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
    // cppcheck-suppress noExplicitConstructor
    ModuleConfiguration(const char *display_name_key)
        : display_name_key(display_name_key) {
    }

    /**
     * Constructor
     */
    // cppcheck-suppress noExplicitConstructor
    ModuleConfiguration(const char *display_name_key, EncodedMessage *message)
        : display_name_key(display_name_key), message(message) {
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
    ModuleConfiguration(const char *display_name_key, EncodedMessage *message, ModuleOrder order)
        : display_name_key(display_name_key), message(message), service_order(order) {
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

struct ModuleReading {
    float calibrated;
    float uncalibrated;

    ModuleReading() : calibrated(0.0), uncalibrated(0.0) {
    }

    ModuleReading(float one_value) : calibrated(one_value), uncalibrated(0.0) {
    }

    ModuleReading(float uncalibrated, float calibrated) : calibrated(calibrated), uncalibrated(uncalibrated) {
    }
};

class ModuleReadings {
public:
    virtual size_t size() const = 0;
    virtual void set(int32_t i, ModuleReading value) = 0;
    virtual ModuleReading get(int32_t i) const = 0;
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
    ModulePosition position;
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

inline const char *get_module_status_string(ModuleStatus ms) {
    switch (ms) {
    case ModuleStatus::Unknown:
        return "unknown";
    case ModuleStatus::Empty:
        return "empty";
    case ModuleStatus::Found:
        return "found";
    case ModuleStatus::Ok:
        return "ok";
    case ModuleStatus::Warning:
        return "warning";
    case ModuleStatus::Fatal:
        return "fatal";
    default:
        return "unknown!";
    }
}

struct ConstructedModule {
    FoundModule found;
    ModuleMetadata const *meta;
    Module *module;
    ModuleStatus status;
    ModuleConfiguration configuration;
};

using ConstructedModulesCollection = collection<ConstructedModule>;

}
