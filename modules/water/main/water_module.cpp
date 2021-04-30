#include "water_module.h"
#include "platform.h"
#include "modules/eeprom.h"

#include "mcp_2803.h"
#include "ads_1219.h"

namespace fk {

FK_DECLARE_LOGGER("water");

#define FK_MCP2803_ADDRESS      0x22
#define FK_ADS1219_ADDRESS      0x41

#define FK_MCP2803_IODIR        0b00000010
#define FK_MCP2803_GPPU         0b00000010
#define FK_MCP2803_GPIO_ON_A    0b00000000
#define FK_MCP2803_GPIO_ON_B    0b00000001

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

            if (!mcp2803_.read_gpio(bus, gpio)) {
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

ModuleReturn WaterModule::initialize(ModuleContext mc, Pool &pool) {
    // TODO Not a fan of this, move to ctor?
    FK_ASSERT(pool_ == nullptr);

    pool_ = &pool;

    loginfo("water: initialize");

    load_configuration(mc, pool);

    auto &bus = mc.module_bus();

    Mcp2803 mcp{ FK_MCP2803_ADDRESS };

    if (!mcp.begin(bus, FK_MCP2803_IODIR, FK_MCP2803_GPPU, FK_MCP2803_GPIO_ON_A)) {
        logerror("mcp2803::begin");
        return { ModuleStatus::Fatal };
    }

    fk_delay(100);

    if (!mcp.begin(bus, FK_MCP2803_IODIR, FK_MCP2803_GPPU, FK_MCP2803_GPIO_ON_B)) {
        logerror("mcp2803::begin");
        return { ModuleStatus::Fatal };
    }

    Mcp2803ReadyChecker ready_checker{ mcp };
    Ads1219 ads{ FK_ADS1219_ADDRESS, &ready_checker };
    if (!ads.begin(bus)) {
        logerror("ads1219::begin");
        return { ModuleStatus::Fatal };
    }

    if (!ads.set_voltage_reference(bus, Ads1219VoltageReference::EXTERNAL)) {
        logerror("ads1219::set_vref");
        return { ModuleStatus::Fatal };
    }

    return { ModuleStatus::Ok };
}

bool WaterModule::load_configuration(ModuleContext mc, Pool &pool) {
    return true;
}

ModuleReturn WaterModule::api(ModuleContext mc, HttpServerConnection *connection, Pool &pool) {
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

    Mcp2803 mcp{ FK_MCP2803_ADDRESS };
    Mcp2803ReadyChecker ready_checker{ mcp };
    Ads1219 ads{ FK_ADS1219_ADDRESS, &ready_checker };

    int32_t value = 0;
    if (!ads.read_differential_0_1(bus, value)) {
        logerror("ads1219::read_diff_0_1");
        return nullptr;
    }

    auto reading = ((float)value * 3.3f / 8388608.0f) / 1.61f;

    loginfo("water: %f", reading);

    mr->set(nreadings++, reading);

    return mr;
}

} // namespace fk
