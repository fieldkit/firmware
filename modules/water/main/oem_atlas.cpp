#include "oem_atlas.h"
#include "platform.h"

namespace fk {

FK_DECLARE_LOGGER("atlas");

static constexpr uint8_t EC_DEFAULT_ADDRESS = 0x64;
static constexpr uint8_t TEMP_DEFAULT_ADDRESS = 0x68;
static constexpr uint8_t PH_DEFAULT_ADDRESS = 0x65;
static constexpr uint8_t DO_DEFAULT_ADDRESS = 0x67;
static constexpr uint8_t ORP_DEFAULT_ADDRESS = 0x66;

struct AtlasRegisters {
    static constexpr uint8_t DEVICE_TYPE = 0x00;
    static constexpr uint8_t LED = 0x05;
    static constexpr uint8_t EC_PROBE_TYPE = 0x08;
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

constexpr uint8_t AtlasHigh = 1;
constexpr uint8_t AtlasLow = 0;

Config config(AtlasSensorType type) {
    switch (type) {
    case AtlasSensorType::Ec:   return { true, 0x18, 0x07, 0x06, 3, 100.0f, "ec" };
    case AtlasSensorType::Ph:   return { true, 0x16, 0x07, 0x06, 1, 1000.0f, "ph" };
    case AtlasSensorType::Do:   return { true, 0x22, 0x07, 0x06, 1, 100.0f, "do" };
    case AtlasSensorType::Temp: return { true, 0x0E, 0x07, 0x06, 1, 1000.0f, "temp" };
    case AtlasSensorType::Orp:  return { true, 0x0E, 0x07, 0x06, 1, 10.0f, "orp" };
    default: {
        break;
    }
    }
    return { false, 0, 0, 0, 0, 1.0f, "<unknown>" };
}

OemAtlas::OemAtlas(TwoWireWrapper &bus) : bus_(&bus) {
}

OemAtlas::OemAtlas(TwoWireWrapper &bus, uint8_t address, AtlasSensorType type) : bus_(&bus), address_(address), type_(type) {
}

bool OemAtlas::find() {
    for (auto address : { EC_DEFAULT_ADDRESS, TEMP_DEFAULT_ADDRESS, PH_DEFAULT_ADDRESS, DO_DEFAULT_ADDRESS, ORP_DEFAULT_ADDRESS }) {
        if (find(address)) {
            return true;
        }
    }

    for (uint8_t address = 1; address < 128; ++address) {
        if (find(address)) {
            return true;
        }
    }

    logerror("error reading device type");

    return false;
}

bool OemAtlas::find(uint8_t address) {
    TwoWire16 d16;
    if (!I2C_CHECK(bus_->read_register_buffer(address, AtlasRegisters::DEVICE_TYPE, (uint8_t *)&d16, sizeof(d16)))) {
        return false;
    }

    auto type = (AtlasSensorType)d16.bytes[0];
    auto cfg = config(type);
    if (!cfg.valid) {
        logerror("[0x%x] unknown device type (0x%x)", address, type);
        return false;
    }

    address_ = address;
    type_ = type;

    loginfo("[0x%x] detected type = %d version = %d '%s'", address, d16.bytes[0], d16.bytes[1], cfg.name);

    return true;
}

const char *OemAtlas::name() const {
    return config(type_).name;
}

AtlasSensorType OemAtlas::type() const {
    return type_;
}

uint8_t OemAtlas::address() const {
    return address_;
}

bool OemAtlas::wake() {
    auto cfg = config(type_);
    return I2C_CHECK(bus_->write_register_u8(address_, cfg.active_register, AtlasHigh));
}

bool OemAtlas::hibernate() {
    auto cfg = config(type_);
    return I2C_CHECK(bus_->write_register_u8(address_, cfg.active_register, AtlasLow));
}

bool OemAtlas::has_reading(uint8_t &has_reading) {
    auto cfg = config(type_);
    return I2C_CHECK(bus_->read_register_u8(address_, cfg.reading_register, has_reading));
}

bool OemAtlas::leds(bool on) {
    return I2C_CHECK(bus_->write_register_u8(address_, AtlasRegisters::LED, on ? AtlasHigh : AtlasLow));
}

bool OemAtlas::read(float *values, size_t &number_of_values) {
    uint8_t has_reading = 0xff;
    auto cfg = config(type_);
    auto started = fk_uptime();
    while (fk_uptime() - started < ATLAS_READINGS_TIMEOUT) {
        if (!I2C_CHECK(bus_->read_register_u8(address_, cfg.reading_register, has_reading))) {
            logerror("error reading register");
            return false;
        }

        if (has_reading) {
            break;
        }

        fk_delay(50);
    }

    if (!has_reading) {
        logerror("reading took too long");
        return false;
    }

    TwoWire32 raw[ATLAS_MAXIMUM_VALUES] = { { 0 }, { 0 }, { 0 } };
    if (!I2C_CHECK(bus_->read_register_buffer(address_, cfg.value_register, (uint8_t *)raw, cfg.number_of_values * sizeof(TwoWire32)))) {
        logerror("error reading values");
        return false;
    }

    if (!I2C_CHECK(bus_->write_register_u8(address_, cfg.reading_register, AtlasLow))) {
        logerror("error clearing reading");
        return false;
    }

    for (auto i = 0; i < cfg.number_of_values; ++i) {
        float value = __builtin_bswap32(raw[i].u32);
        value /= cfg.divisor;
        values[i] = value;
    }

    number_of_values = cfg.number_of_values;

    return true;
}

}
