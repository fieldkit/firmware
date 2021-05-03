#include "water_module.h"
#include "water_api.h"
#include "platform.h"
#include "modules/eeprom.h"
#include "curves.h"

namespace fk {

FK_DECLARE_LOGGER("water");

#define FK_MCP2803_ADDRESS          0x22
#define FK_ADS1219_ADDRESS          0x41

#define FK_MCP2803_IODIR            0b00000010
#define FK_MCP2803_GPPU             0b00000010

#define FK_MCP2803_GPIO_ON          0b00000001
#define FK_MCP2803_GPIO_OFF         0b00000000

#define FK_MCP2803_GPIO_EXCITE_ON   0b00000101
#define FK_MCP2803_GPIO_EXCITE_OFF  0b00000001

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

            if (!(gpio & 0x2)) {
                return true;
            }

            logdebug("gpio: 0x%x", gpio);

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
    loginfo("water: initialize");

    load_configuration(mc, pool);

    auto &bus = mc.module_bus();

    Mcp2803 mcp{ bus, FK_MCP2803_ADDRESS };
    if (!mcp.configure(FK_MCP2803_IODIR, FK_MCP2803_GPPU, FK_MCP2803_GPIO_OFF)) {
        logerror("mcp2803::begin");
        return { ModuleStatus::Fatal };
    }

    fk_delay(100);

    if (!mcp.configure(FK_MCP2803_IODIR, FK_MCP2803_GPPU, FK_MCP2803_GPIO_ON)) {
        logerror("mcp2803::begin");
        return { ModuleStatus::Fatal };
    }

    Mcp2803ReadyChecker ready_checker{ mcp };
    Ads1219 ads{ bus, FK_ADS1219_ADDRESS, &ready_checker };
    if (!ads.begin()) {
        logerror("ads1219::begin");
        return { ModuleStatus::Fatal };
    }

    if (!ads.configure(Ads1219VoltageReference::External, Ads1219Channel::Diff_0_1)) {
        logerror("ads1219::configure");
        return { ModuleStatus::Fatal };
    }

    return { ModuleStatus::Ok };
}

bool WaterModule::load_configuration(ModuleContext mc, Pool &pool) {
    ModuleEeprom eeprom{ mc.module_bus() };

    cfg_message_ = nullptr;
    cfg_ = nullptr;
    pool_->clear();

    size_t size = 0;
    auto buffer = (uint8_t *)pool.malloc(MaximumConfigurationSize);
    bzero(buffer, MaximumConfigurationSize);
    if (!eeprom.read_configuration(buffer, size, MaximumConfigurationSize)) {
        logwarn("error reading configuration");
    } else if (size > 0) {
        auto cfg = fk_module_configuration_decoding_new(pool_);
        auto stream = pb_istream_from_buffer(buffer, size);
        if (!pb_decode_delimited(&stream, fk_data_ModuleConfiguration_fields, cfg)) {
            logerror("mod-cfg: error decoding ");
            return false;
        }
        else {
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
        logerror("error handling api (ms::fatal)");
        return { ModuleStatus::Fatal };
    }

    load_configuration(mc, pool);

    return { ModuleStatus::Ok };
}

ModuleReturn WaterModule::service(ModuleContext mc, Pool &pool) {
    return { ModuleStatus::Ok };
}

ModuleSensors const *WaterModule::get_sensors(Pool &pool) {
    auto voltage = pool.malloc_with<SensorMetadata>({
        .name = "v",
        .unitOfMeasure = "V",
        .flags = 0,
    });
    return pool.malloc_with<ModuleSensors>({
        .nsensors = 1,
        .sensors = voltage,
    });
}

const char *WaterModule::get_display_name_key() {
    return "modules.water.unknown";
}

ModuleConfiguration const WaterModule::get_configuration(Pool &pool) {
    return { get_display_name_key(), ModulePower::ReadingsOnly, 0, cfg_message_, ModuleOrderProvidesCalibration };
}

ModuleReadings *WaterModule::take_readings(ReadingsContext mc, Pool &pool) {
    auto mr = new(pool) NModuleReadings<1>();
    auto nreadings = 0u;

    auto &bus = mc.module_bus();

    Mcp2803 mcp{ bus, FK_MCP2803_ADDRESS };
    Mcp2803ReadyChecker ready_checker{ mcp };
    Ads1219 ads{ bus, FK_ADS1219_ADDRESS, &ready_checker };

    if (!ads.configure(Ads1219VoltageReference::External, Ads1219Channel::Diff_0_1)) {
        logerror("ads1219::configure");
        return nullptr;
    }

    auto excite_duration = mc.position().integer() == 2 ? 10u : 0u;
    if (excite_duration > 0) {
        loginfo("excitation: %" PRIu32 "ms", excite_duration);

        if (!excite_control(mcp, true)) {
            return nullptr;
        }

        fk_delay(excite_duration);

        if (!excite_control(mcp, false)) {
            return nullptr;
        }

        fk_delay(excite_duration);

        if (!excite_control(mcp, true)) {
            return nullptr;
        }
    }
    else {
        loginfo("excitation: disabled");
    }

    int32_t value = 0;
    if (!ads.read(value)) {
        logerror("ads1219::read_diff_0_1");
        return nullptr;
    }

    if (excite_duration > 0) {
        if (!excite_control(mcp, false)) {
            return nullptr;
        }
    }

    auto reading = ((float)value * 3.3f / 8388608.0f);

    loginfo("water: %f", reading);

    auto curve = create_curve(cfg_, pool);

    mr->set(nreadings++, ModuleReading{ reading, curve->apply(reading) });

    return mr;
}

bool WaterModule::excite_control(Mcp2803 &mcp, bool high)  {
    if (!mcp.configure(FK_MCP2803_IODIR, FK_MCP2803_GPPU, high ? FK_MCP2803_GPIO_EXCITE_ON : FK_MCP2803_GPIO_EXCITE_OFF)) {
        logerror("mcp2803::configure-excite");
        return false;
    }
    return true;
}

} // namespace fk
