#include "water_module.h"

using namespace fk;

FK_DECLARE_LOGGER("water");

struct AtlasRegisters {
    static constexpr uint8_t DEVICE_TYPE = 0x00;
    static constexpr uint8_t LED = 0x05;
    static constexpr uint8_t IRQ = 0x04;

    static constexpr uint8_t EC_PROBE_TYPE = 0x08;

    static constexpr uint8_t EC_DEFAULT_ADDRESS = 0x64;
    static constexpr uint8_t TEMP_DEFAULT_ADDRESS = 0x68;
    static constexpr uint8_t PH_DEFAULT_ADDRESS = 0x65;
    static constexpr uint8_t DO_DEFAULT_ADDRESS = 0x67;
    static constexpr uint8_t ORP_DEFAULT_ADDRESS = 0x66;
};

union TwoWire16 {
    uint8_t bytes[2];
    uint16_t u16;
};

union TwoWire32 {
    uint8_t bytes[4];
    uint32_t u32;
};

struct Config {
    uint8_t valid;
    uint8_t value_register;
    uint8_t reading_register;
    uint8_t active_register;
    uint8_t number_of_values;
    float divisor;
    const char *name;
};

enum class AtlasSensorType : uint8_t {
    Unknown = 0xff,
    Ph = 1,
    Ec = 4,
    Orp = 2,
    Do = 3,
    Temp = 5
};

constexpr uint8_t AtlasHigh = 1;
constexpr uint8_t AtlasLow = 0;

Config config(AtlasSensorType type) {
    switch (type) {
    case AtlasSensorType::Ec:   return { true, 0x18, 0x07, 0x06, 3, 100.0f, "ec" };
    case AtlasSensorType::Ph:   return { true, 0x16, 0x07, 0x06, 1, 1000.0f, "ph" };
    case AtlasSensorType::Do:   return { true, 0x22, 0x07, 0x06, 1, 100.0f, "do" };
    case AtlasSensorType::Temp: return { true, 0x0E, 0x07, 0x06, 1, 1000.0f, "temp" };
    case AtlasSensorType::Orp:  return { true, 0x0E, 0x07, 0x06, 1, 10.0f, "orp" };
    }
    return { false, 0, 0, 0, 0, 1.0f, "<unknown>" };
}

bool WaterModule::initialize(ModuleContext mc, fk::Pool &pool) {
    loginfo("initialize");
    return true;
}

ModuleReadings *WaterModule::take_readings(ModuleContext mc, fk::Pool &pool) {
    auto address = AtlasRegisters::PH_DEFAULT_ADDRESS;
    auto &bus = mc.module_bus();

    TwoWire16 d16;
    if (bus.read_register_buffer(address, AtlasRegisters::DEVICE_TYPE, (uint8_t *)&d16, sizeof(d16)) != 0) {
        logerror("error reading device type");
        return nullptr;
    }

    auto type = (AtlasSensorType)d16.bytes[0];
    auto cfg = config(type);
    if (!cfg.valid) {
        logerror("unknown device type");
        return nullptr;
    }

    loginfo("detected '%s' type = %d version = %d", cfg.name, d16.bytes[0], d16.bytes[1]);

    uint8_t active = 0xff;
    if (bus.read_register_u8(address, cfg.active_register, active) != 0) {
        logerror("error reading active status");
        return nullptr;
    }

    if (!active) {
        if (bus.write_register_u8(address, cfg.active_register, AtlasHigh) != 0) {
            return nullptr;
        }
    }

    uint8_t has_reading = 0xff;
    if (bus.read_register_u8(address, cfg.reading_register, has_reading) != 0) {
        logerror("error reading register");
        return nullptr;
    }

    if (!has_reading) {
        logerror("no readings");
        return nullptr;
    }

    TwoWire32 values[3];
    if (bus.read_register_buffer(address, cfg.value_register, (uint8_t *)&values, cfg.number_of_values * sizeof(TwoWire32)) != 0) {
        logerror("error reading values");
        return nullptr;
    }

    if (bus.write_register_u8(address, cfg.reading_register, AtlasLow) != 0) {
        logerror("error clearing reading");
        return nullptr;
    }

    if (bus.write_register_u8(address, cfg.active_register, AtlasLow) != 0) {
        logerror("error hibernating");
        return nullptr;
    }

    auto mr = new(pool) NModuleReadings<3>(1);
    for (auto i = 0; i < cfg.number_of_values; ++i) {
        float value = values[i].u32;
        value /= cfg.divisor;
        loginfo("atlas(0x%x, %s) -> (value = %f) (raw = %lu)", address, cfg.name, value, values[i].u32);
        mr->set(i, value);
    }

    return mr;
}
