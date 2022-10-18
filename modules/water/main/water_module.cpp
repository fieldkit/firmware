#include "water_module.h"
#include "modules/eeprom.h"
#include "platform.h"
#include "state_ref.h"
#include "water_api.h"
#include "mpl3115a2.h"
#include "ready_checkers.h"
#include "water_protocol.h"

namespace fk {

FK_DECLARE_LOGGER("water");

WaterModule::WaterModule(Pool &pool) : pool_(pool.subpool("water", MaximumConfigurationSize)) {
}

WaterModule::~WaterModule() {
}

bool WaterModule::load_configuration(ModuleContext mc, Pool &pool) {
    ModuleEeprom eeprom{ mc.module_bus() };

    logdebug("reading header");

    // We need the header to know the kind of module we are so if that
    // fails then we're in pretty bad shape.
    bzero(&header_, sizeof(ModuleHeader));
    if (!eeprom.read_header(header_)) {
        logwarn("error reading header");
        return false;
    }

    if (!fk_module_header_valid(&header_)) {
        logwarn("invalid header");
        return false;
    }

    loginfo("have header: mk=%02" PRIx32 "%02" PRIx32, header_.manufacturer, header_.kind);

    cfg_message_ = nullptr;
    cfg_ = nullptr;
    pool_->clear();

    size_t size = 0;
    auto buffer = (uint8_t *)pool.malloc(MaximumConfigurationSize);
    bzero(buffer, MaximumConfigurationSize);
    if (!eeprom.read_configuration(buffer, size, MaximumConfigurationSize)) {
        logwarn("mod-cfg: reading");
    } else if (size > 0) {
        auto cfg = fk_module_configuration_decoding_new(pool_);
        auto stream = pb_istream_from_buffer(buffer, size);
        if (!pb_decode_delimited(&stream, fk_data_ModuleConfiguration_fields, cfg)) {
            // Some modules consider this an error. We continue along uncalibrated.
            logwarn("mod-cfg: decoding");
        } else {
            loginfo("mod-cfg: decoded");
            cfg_message_ = pool_->wrap_copy(buffer, size);
            cfg_ = cfg;
        }
    }

    return true;
}

ModuleReturn WaterModule::api(ModuleContext mc, HttpServerConnection *connection, Pool &pool) {
    WaterApi api;
    if (!api.handle(mc, connection, pool)) {
        logerror("api (ms::fatal)");
        return { ModuleStatus::Fatal };
    }

    load_configuration(mc, pool);

    return { ModuleStatus::Ok };
}

ModuleReturn WaterModule::service(ModuleContext mc, Pool &pool) {
    return { ModuleStatus::Ok };
}

static SensorMetadata const fk_module_water_do_sensor_metas[] = {
    { .name = "do", .unitOfMeasure = "%", .flags = 0 },
    { .name = "temperature", .unitOfMeasure = "°C", .flags = FK_MODULES_FLAG_INTERNAL },
    { .name = "pressure", .unitOfMeasure = "kPa", .flags = FK_MODULES_FLAG_INTERNAL },
};

static ModuleSensors fk_module_water_do_sensors = {
    .nsensors = sizeof(fk_module_water_do_sensor_metas) / sizeof(SensorMetadata),
    .sensors = fk_module_water_do_sensor_metas,
};

ModuleSensors const *WaterModule::get_sensors(Pool &pool) {
    SensorMetadata *sensors = nullptr;

    switch (header_.kind) {
    case FK_MODULES_KIND_WATER_PH:
        sensors = pool.malloc_with<SensorMetadata>({
            .name = "ph",
            .unitOfMeasure = "pH",
            .flags = 0,
        });
        break;
    case FK_MODULES_KIND_WATER_EC:
        sensors = pool.malloc_with<SensorMetadata>({
            .name = "ec",
            .unitOfMeasure = "µS/cm",
            .flags = 0,
        });
        break;
    case FK_MODULES_KIND_WATER_DO:
        return &fk_module_water_do_sensors;
    case FK_MODULES_KIND_WATER_TEMP:
        sensors = pool.malloc_with<SensorMetadata>({
            .name = "temp",
            .unitOfMeasure = "°C",
            .flags = 0,
        });
        break;
    case FK_MODULES_KIND_WATER_ORP:
        sensors = pool.malloc_with<SensorMetadata>({
            .name = "orp",
            .unitOfMeasure = "mV",
            .flags = 0,
        });
        break;
    default:
        logwarn("unknown water module kind: %d", header_.kind);
        return nullptr;
    };

    return pool.malloc_with<ModuleSensors>({
        .nsensors = 1,
        .sensors = sensors,
    });
}

const char *WaterModule::get_display_name_key() {
    switch (header_.kind) {
    case FK_MODULES_KIND_WATER_PH:
        return "modules.water.ph";
    case FK_MODULES_KIND_WATER_EC:
        return "modules.water.ec";
    case FK_MODULES_KIND_WATER_DO:
        return "modules.water.do";
    case FK_MODULES_KIND_WATER_TEMP:
        return "modules.water.temp";
    case FK_MODULES_KIND_WATER_ORP:
        return "modules.water.orp";
    default:
        return "modules.water.unknown";
    };
}

ModuleConfiguration const WaterModule::get_configuration(Pool &pool) {
    switch (header_.kind) {
    case FK_MODULES_KIND_WATER_TEMP:
        return ModuleConfiguration{ get_display_name_key(), ModulePower::ReadingsOnly, cfg_message_, ModuleOrderProvidesCalibration };
    };
    return ModuleConfiguration{ get_display_name_key(), ModulePower::ReadingsOnly, cfg_message_, DefaultModuleOrder };
}

bool WaterModule::can_enable() {
    return lockout_.can_enable();
}

ModuleReturn WaterModule::initialize(ModuleContext mc, Pool &pool) {
    loginfo("initialize");

    if (!load_configuration(mc, pool)) {
        return { ModuleStatus::Fatal };
    }

    auto &bus = mc.module_bus();
    WaterProtocol water_protocol{ pool, bus, WaterModality::PH };

    if (!water_protocol.initialize()) {
        return { ModuleStatus::Fatal };
    }

    return { ModuleStatus::Ok };
}

ModuleReadings *WaterModule::take_readings(ReadingsContext mc, Pool &pool) {
    if (!lockout_.try_enable(mc.position())) {
        return new (pool) EmptyReadings();
    }

    auto uptime = fk_uptime();

    auto &bus = mc.module_bus();
    WaterProtocol water_protocol{ pool, bus, WaterModality::PH };

    if (!water_protocol.initialize()) {
        logwarn("water-proto: initialize error");
        return nullptr;
    }

    auto water_readings = water_protocol.take_readings(mc, cfg_, pool);
    if (water_readings == nullptr) {
        return nullptr;
    }

    auto has_mpl = header_.kind == FK_MODULES_KIND_WATER_DO;
    ModuleReadings *mr = has_mpl ? (ModuleReadings *)new (pool) NModuleReadings<3>() : (ModuleReadings *)new (pool) NModuleReadings<1>();
    mr->set(0, SensorReading{ mc.now(), water_readings->uncalibrated, water_readings->calibrated, water_readings->factory });

    if (has_mpl) {
        Mpl3115a2 mpl3115a2{ bus };

        if (mpl3115a2.begin()) {
            Mpl3115a2Reading reading;
            if (mpl3115a2.get(&reading)) {
                mr->set(1, SensorReading{ mc.now(), reading.temperature });
                mr->set(2, SensorReading{ mc.now(), reading.pressure });
            } else {
                logerror("mpl3115a2 get");
            }
        } else {
            logerror("mpl3115a2 begin");
        }
    }

#if defined(FK_WATER_LOCKOUT_ALL_MODULES)
    lockout_.enable_until_uptime(uptime + OneMinuteMs);
#else
    if (water_protocol.lockout_enabled()) {
        lockout_.enable_until_uptime(uptime + OneMinuteMs);
    }
#endif

    return mr;
}

} // namespace fk
