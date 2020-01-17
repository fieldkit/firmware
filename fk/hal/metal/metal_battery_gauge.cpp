#include "hal/metal/metal_battery_gauge.h"
#include "platform.h"

#include <Wire.h>

namespace fk {

FK_DECLARE_LOGGER("battery");

constexpr uint8_t INA219_ADDRESS = 0x40;
constexpr uint8_t INA219_REGISTER_CONFIG = 0x00;
constexpr uint8_t INA219_REGISTER_VOLTAGE_SHUNT = 0x01;
constexpr uint8_t INA219_REGISTER_VOLTAGE_BUS = 0x02;
constexpr uint8_t INA219_REGISTER_POWER = 0x03;
constexpr uint8_t INA219_REGISTER_CURRENT = 0x04;
constexpr uint8_t INA219_REGISTER_CALIBRATION = 0x05;

constexpr uint16_t INA219_CONFIG_RESET = 0x8000;
constexpr uint16_t INA219_CONFIG_BVOLTAGERANGE_MASK = 0x2000;

enum {
    INA219_CONFIG_BVOLTAGERANGE_16V = 0x0000,
    INA219_CONFIG_BVOLTAGERANGE_32V = 0x2000,
};

constexpr uint16_t INA219_CONFIG_GAIN_MASK = 0x1800;

enum {
    INA219_CONFIG_GAIN_1_40MV = 0x0000,  // Gain 1, 40mV Range
    INA219_CONFIG_GAIN_2_80MV = 0x0800,  // Gain 2, 80mV Range
    INA219_CONFIG_GAIN_4_160MV = 0x1000, // Gain 4, 160mV Range
    INA219_CONFIG_GAIN_8_320MV = 0x1800, // Gain 8, 320mV Range
};

constexpr uint16_t INA219_CONFIG_BADCRES_MASK = 0x0780;

enum {
    INA219_CONFIG_BADCRES_9BIT = (0x0000),  // 9-bit bus res = 0..511
    INA219_CONFIG_BADCRES_10BIT = (0x0080), // 10-bit bus res = 0..1023
    INA219_CONFIG_BADCRES_11BIT = (0x0100), // 11-bit bus res = 0..2047
    INA219_CONFIG_BADCRES_12BIT = (0x0180), // 12-bit bus res = 0..4097
};

constexpr uint16_t INA219_CONFIG_SADCRES_MASK = 0x0078;

enum {
    INA219_CONFIG_SADCRES_9BIT_1S_84US = 0x0000,     // 1 x 9-bit shunt sample
    INA219_CONFIG_SADCRES_10BIT_1S_148US = 0x0008,   // 1 x 10-bit shunt sample
    INA219_CONFIG_SADCRES_11BIT_1S_276US = 0x0010,   // 1 x 11-bit shunt sample
    INA219_CONFIG_SADCRES_12BIT_1S_532US = 0x0018,   // 1 x 12-bit shunt sample
    INA219_CONFIG_SADCRES_12BIT_2S_1060US = 0x0048,  // 2 x 12-bit shunt samples averaged together
    INA219_CONFIG_SADCRES_12BIT_4S_2130US = 0x0050,  // 4 x 12-bit shunt samples averaged together
    INA219_CONFIG_SADCRES_12BIT_8S_4260US = 0x0058,  // 8 x 12-bit shunt samples averaged together
    INA219_CONFIG_SADCRES_12BIT_16S_8510US = 0x0060, // 16 x 12-bit shunt samples averaged together
    INA219_CONFIG_SADCRES_12BIT_32S_17MS = 0x0068,   // 32 x 12-bit shunt samples averaged together
    INA219_CONFIG_SADCRES_12BIT_64S_34MS = 0x0070,   // 64 x 12-bit shunt samples averaged together
    INA219_CONFIG_SADCRES_12BIT_128S_69MS = 0x0078,  // 128 x 12-bit shunt samples averaged together
};

constexpr uint16_t INA219_CONFIG_MODE_MASK = 0x0007;

enum {
    INA219_CONFIG_MODE_POWERDOWN,
    INA219_CONFIG_MODE_SVOLT_TRIGGERED,
    INA219_CONFIG_MODE_BVOLT_TRIGGERED,
    INA219_CONFIG_MODE_SANDBVOLT_TRIGGERED,
    INA219_CONFIG_MODE_ADCOFF,
    INA219_CONFIG_MODE_SVOLT_CONTINUOUS,
    INA219_CONFIG_MODE_BVOLT_CONTINUOUS,
    INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS
};

static bool ina219_write(TwoWireWrapper &bus, uint8_t reg, uint16_t value) {
    uint8_t data[] = {
        reg,
        (uint8_t)((value >> 8) & 0xff),
        (uint8_t)((value     ) & 0xff),
    };
    if (!I2C_CHECK(bus.write(INA219_ADDRESS, data, sizeof(data)))) {
        return false;
    }

    return true;
}

static bool ina219_read(TwoWireWrapper &bus, uint8_t reg, uint16_t &value) {
    uint8_t data[2];

    if (!I2C_CHECK(bus.read_register_buffer(INA219_ADDRESS, reg, data, sizeof(data)))) {
        return false;
    }

    // NOTE: Conversion time is 586us
    fk_delay(1);

    value = (data[0] << 8) | data[1];

    return true;
}

MetalBatteryGauge::MetalBatteryGauge() {
}

bool MetalBatteryGauge::begin() {
    status_ = Availability::Unavailable;

    auto bus = get_board()->i2c_module();

    // 16V 400ma
    calibration_value_ = 8192;
    ma_divider_ = 20.0f;
    power_multiplier_ = 1.0f;
    config_ = INA219_CONFIG_BVOLTAGERANGE_16V | INA219_CONFIG_GAIN_1_40MV | INA219_CONFIG_BADCRES_12BIT | INA219_CONFIG_SADCRES_12BIT_1S_532US | INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS;

    // 32V 1A
    calibration_value_ = 10240;
    ma_divider_ = 25.0f;
    power_multiplier_ = 0.8f;
    config_ = INA219_CONFIG_BVOLTAGERANGE_32V | INA219_CONFIG_GAIN_8_320MV | INA219_CONFIG_BADCRES_12BIT | INA219_CONFIG_SADCRES_12BIT_1S_532US | INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS;

    if (!ina219_write(bus, INA219_REGISTER_CALIBRATION, calibration_value_)) {
        return false;
    }
    if (!ina219_write(bus, INA219_REGISTER_CONFIG, config_)) {
        return false;
    }

    status_ = Availability::Available;

    return true;
}

BatteryReading MetalBatteryGauge::get() {
    if (status_ != Availability::Available) {
        return { .available = false };
    }

    uint16_t value;

    auto bus = get_board()->i2c_module();

    // Sometimes a sharp load will reset the INA219, which will
    // reset the cal register, meaning CURRENT and POWER will
    // not be available ... avoid this by always setting a cal
    // value even if it's an unfortunate extra step
    if (!ina219_write(bus, INA219_REGISTER_CALIBRATION, calibration_value_)) {
        return { .available = false };
    }

    if (!ina219_read(bus, INA219_REGISTER_VOLTAGE_BUS, value)) {
        return { .available = false };
    }
    // Shift to the right 3 to drop CNVR and OVF and multiply by LSB
    // auto overflow = (value & 0x1);
    // auto ready = (value & 0x2);
    int32_t bus_voltage_raw = (int32_t)(int16_t)((value >> 3) * 4);

    if (!ina219_read(bus, INA219_REGISTER_VOLTAGE_SHUNT, value)) {
        return { .available = false };
    }
    int32_t shunted_voltage_raw = (int32_t)(int16_t)value;

    if (!ina219_read(bus, INA219_REGISTER_CURRENT, value)) {
        return { .available = false };
    }
    int32_t ma_raw = (int32_t)(int16_t)value;

    if (!ina219_read(bus, INA219_REGISTER_POWER, value)) {
        return { .available = false };
    }
    int32_t power_raw = (int32_t)(int16_t)value;

    float bus_voltage = (float)bus_voltage_raw * 0.001f;
    float shunted_voltage = (float)shunted_voltage_raw * 0.01f;
    float ma = (float)ma_raw / (float)ma_divider_;
    float power = (float)power_raw * (float)power_multiplier_;

    return {
        .available = true,
        .bus_voltage = bus_voltage,
        .shunted_voltage = shunted_voltage,
        .ma = ma,
        .power = power,
    };
}

} // namespace fk
