#pragma once

#include "common.h"
#include "collections.h"
#include "modules/bridge/meta.h"
#include "modules/shared/uuid.h"
#include "modules/scanning.h"

namespace fk {

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
constexpr ModuleOrder ModuleOrderInternal = DefaultModuleOrder - 10;

/**
 * Module order for modules that provide calibration data other
 * modules may use.
 */
constexpr ModuleOrder ModuleOrderProvidesCalibration = DefaultModuleOrder - 1;

typedef struct ModuleTiming {
    /**
     * Required interval between readings.
     */
    uint32_t readings_interval{ 0 };

    /**
     * How long to wait after powering up the module.
     */
    uint32_t wake_delay{ 100 };

    /**
     * Required interval between being serviced.
     */
    uint32_t service_interval{ 0 };

    ModuleTiming() {
    }

    ModuleTiming(uint32_t readings_interval) : readings_interval(readings_interval) {
    }
} ModuleTiming;

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
     * Intervals and delays.
     **/
    ModuleTiming timing;

    /**
     * Preferred module servicing order. This is mostly a hint to the
     * core when to service this module. Most modules will have an
     * average order that effectively means they'll be serviced in
     * physical module order.
     */
    ModuleOrder service_order{ DefaultModuleOrder };

    /**
     * Constructor
     */
    ModuleConfiguration() {
    }

    /**
     * Constructor
     */
    // cppcheck-suppress noExplicitConstructor
    ModuleConfiguration(const char *display_name_key) : display_name_key(display_name_key) {
    }

    /**
     * Constructor
     */
    // cppcheck-suppress noExplicitConstructor
    ModuleConfiguration(const char *display_name_key, EncodedMessage *message) : display_name_key(display_name_key), message(message) {
    }

    /**
     * Constructor
     */
    ModuleConfiguration(const char *display_name_key, ModuleOrder order) : display_name_key(display_name_key), service_order(order) {
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
    ModuleConfiguration(const char *display_name_key, ModulePower power, ModuleTiming timing)
        : display_name_key(display_name_key), power(power), timing(timing) {
    }

    /**
     * Constructor
     */
    ModuleConfiguration(const char *display_name_key, ModulePower power, ModuleTiming timing, ModuleOrder order)
        : display_name_key(display_name_key), power(power), timing(timing), service_order(order) {
    }

    /**
     * Constructor
     */
    ModuleConfiguration(const char *display_name_key, ModulePower power, ModuleTiming timing, EncodedMessage *message, ModuleOrder order)
        : display_name_key(display_name_key), message(message), power(power), timing(timing), service_order(order) {
    }
} ModuleConfiguration;

struct ModuleReading {
    float calibrated;
    float uncalibrated;
    float factory;

    ModuleReading() : calibrated(0.0), uncalibrated(0.0) {
    }

    ModuleReading(float one_value) : calibrated(one_value), uncalibrated(0.0) {
    }

    ModuleReading(float uncalibrated, float calibrated) : calibrated(calibrated), uncalibrated(uncalibrated) {
    }

    ModuleReading(float uncalibrated, float calibrated, float factory)
        : calibrated(calibrated), uncalibrated(uncalibrated), factory(factory) {
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

enum class ModuleStatus { Unknown = 0, Empty, Found, Ok, Warning, Fatal };

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

} // namespace fk
