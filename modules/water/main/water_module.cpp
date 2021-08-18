#include "water_module.h"
#include "curves.h"
#include "modules/eeprom.h"
#include "platform.h"
#include "state_ref.h"
#include "water_api.h"

namespace fk {

FK_DECLARE_LOGGER("water");

#define FK_MCP2803_ADDRESS 0x22
#define FK_ADS1219_ADDRESS 0x45

#define FK_MCP2803_IODIR 0b00000010
#define FK_MCP2803_GPPU 0b00000010

#define FK_MCP2803_GPIO_ON 0b00000001
#define FK_MCP2803_GPIO_OFF 0b00000000

#define FK_MCP2803_GPIO_EXCITE_ON 0b00000101
#define FK_MCP2803_GPIO_EXCITE_OFF 0b00000001

class Mcp2803ReadyChecker : public Ads1219ReadyChecker {
private:
    Mcp2803 &mcp2803_;

public:
    Mcp2803ReadyChecker(Mcp2803 &mcp2803) : mcp2803_(mcp2803) {
    }

public:
    bool block_until_ready(TwoWireWrapper &bus) override {
        auto give_up = fk_uptime() + 1000;
        while (fk_uptime() < give_up) {
            uint8_t gpio{ 0 };

            if (!mcp2803_.read_gpio(gpio)) {
                return false;
            }

            logdebug("gpio: 0x%x", gpio);

            if (!(gpio & 0x2)) {
                return true;
            }

            fk_delay(10);
        }
        return false;
    }
};

WaterModule::WaterModule(Pool &pool) : pool_(pool.subpool("water", MaximumConfigurationSize)) {
}

WaterModule::~WaterModule() {
}

ModuleReturn WaterModule::initialize(ModuleContext mc, Pool &pool) {
    loginfo("initialize");

    if (!load_configuration(mc, pool)) {
        return { ModuleStatus::Fatal };
    }

    auto &bus = mc.module_bus();

    Mcp2803 mcp{ bus, FK_MCP2803_ADDRESS };
    Mcp2803ReadyChecker ready_checker{ mcp };
    Ads1219 ads{ bus, FK_ADS1219_ADDRESS, &ready_checker };

    if (!initialize(mcp, ads)) {
        return { ModuleStatus::Fatal };
    }

    return { ModuleStatus::Ok };
}

bool WaterModule::initialize(Mcp2803 &mcp, Ads1219 &ads) {
    if (!mcp.configure(FK_MCP2803_IODIR, FK_MCP2803_GPPU, FK_MCP2803_GPIO_OFF)) {
        logerror("mcp2803::begin");
        return false;
    }

    if (!mcp.configure(FK_MCP2803_IODIR, FK_MCP2803_GPPU, FK_MCP2803_GPIO_ON)) {
        logerror("mcp2803::begin");
        return false;
    }

    fk_delay(100);

    if (!ads.begin()) {
        logerror("ads1219::begin");
        return false;
    }

    if (!ads.configure(Ads1219VoltageReference::Internal, Ads1219Channel::Diff_0_1, Ads1219Gain::One,
                       Ads1219DataRate::DataRate_20)) {
        logerror("ads1219::configure");
        return false;
    }

    return true;
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
        sensors = pool.malloc_with<SensorMetadata>({
            .name = "do",
            .unitOfMeasure = "mg/L",
            .flags = 0,
        });
        break;
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
        return { get_display_name_key(), ModulePower::ReadingsOnly, 0, cfg_message_, ModuleOrderProvidesCalibration };
    };
    return { get_display_name_key(), ModulePower::ReadingsOnly, 0, cfg_message_, DefaultModuleOrder };
}

uint32_t WaterModule::excite_duration() {
    auto gs = get_global_state_ro();
    auto delay = gs.get()->debugging.ec_excite_delay;
    loginfo("ec delay: %dms", delay);
    switch (header_.kind) {
    case FK_MODULES_KIND_WATER_EC: {
        return delay == 0 ? 10 : delay;
    }
    default:
        return 0;
    };
}

ModuleReadings *WaterModule::take_readings(ReadingsContext mc, Pool &pool) {
    auto &bus = mc.module_bus();

    Mcp2803 mcp{ bus, FK_MCP2803_ADDRESS };
    Mcp2803ReadyChecker ready_checker{ mcp };
    Ads1219 ads{ bus, FK_ADS1219_ADDRESS, &ready_checker };

    if (!initialize(mcp, ads)) {
        return nullptr;
    }

    auto excite_for = excite_duration();
    if (excite_for > 0) {
        loginfo("excitation: %" PRIu32 "ms", excite_for);

        if (!excite_control(mcp, true)) {
            return nullptr;
        }
    } else {
        loginfo("excitation: disabled");
    }

    int32_t value = 0;
    if (!ads.read(value)) {
        logerror("read");
        return nullptr;
    }

    if (excite_for > 0) {
        if (!excite_control(mcp, false)) {
            return nullptr;
        }
    }

    auto curve = create_curve(cfg_, pool);
    auto uncalibrated = ((float)value * 2.048f) / 8388608.0f;
    auto calibrated = curve->apply(uncalibrated);

    loginfo("[%d] water: %f (%f)", mc.position().integer(), uncalibrated, calibrated);

    auto mr = new (pool) NModuleReadings<1>();

    auto nreadings = 0u;
    mr->set(nreadings++, ModuleReading{ uncalibrated, calibrated });

    return mr;
}

bool WaterModule::excite_control(Mcp2803 &mcp, bool high) {
    if (!mcp.configure(FK_MCP2803_IODIR, FK_MCP2803_GPPU,
                       high ? FK_MCP2803_GPIO_EXCITE_ON : FK_MCP2803_GPIO_EXCITE_OFF)) {
        logerror("mcp2803::configure-excite");
        return false;
    }
    return true;
}

} // namespace fk
