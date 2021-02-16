#pragma once

#include <modules/bridge/modules.h>

namespace fk {

static constexpr size_t ATLAS_MAXIMUM_VALUES = 3;
static constexpr size_t ATLAS_READINGS_TIMEOUT = 1000;

enum class AtlasSensorType : uint8_t {
    Unknown = 0xff,
    Ph = 1,
    Ec = 4,
    Orp = 2,
    Do = 3,
    Temp = 5
};

struct CalibrationStatus {
    uint8_t success;
    uint8_t value;
};

struct Compensation {
    optional<float> temperature;
    optional<float> salinity;
    optional<float> pressure;
};

class OemAtlas {
private:
    TwoWireWrapper *bus_;
    uint8_t address_;
    AtlasSensorType type_;

public:
    OemAtlas(TwoWireWrapper &bus);
    OemAtlas(TwoWireWrapper &bus, uint8_t address, AtlasSensorType type);

public:
    bool find();
    bool find(uint8_t address);

public:
    bool wake();
    bool hibernate();
    bool has_reading(uint8_t &has_reading);
    bool leds(bool on);
    bool compensate(Compensation compensation);
    bool read(float *values, size_t &number_of_values);
    const char *name() const;
    AtlasSensorType type() const;
    uint8_t address() const;

public:
    CalibrationStatus calibration();
    CalibrationStatus calibrate(uint8_t which, float reference);
    bool clear_calibration();
    bool finish_calibration();

private:
    bool compensate(uint8_t reg, float value);

};

} // namespace fk
