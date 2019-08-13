#pragma once

#include <modules.h>

namespace fk {

static constexpr size_t ATLAS_MAXIMUM_VALUES = 3;
static constexpr size_t ATLAS_READINGS_TIMEOUT = 1000;
static constexpr uint8_t EC_DEFAULT_ADDRESS = 0x64;
static constexpr uint8_t TEMP_DEFAULT_ADDRESS = 0x68;
static constexpr uint8_t PH_DEFAULT_ADDRESS = 0x65;
static constexpr uint8_t DO_DEFAULT_ADDRESS = 0x67;
static constexpr uint8_t ORP_DEFAULT_ADDRESS = 0x66;

enum class AtlasSensorType : uint8_t {
    Unknown = 0xff,
    Ph = 1,
    Ec = 4,
    Orp = 2,
    Do = 3,
    Temp = 5
};

class OemAtlas {
private:
    TwoWireWrapper *bus_;
    uint8_t address_;
    AtlasSensorType type_;

public:
    OemAtlas(TwoWireWrapper &bus);

public:
    bool find(uint8_t address);

public:
    bool wake();
    bool hibernate();
    bool has_reading(uint8_t &has_reading);
    bool leds(bool on);
    bool read(float *values, size_t &number_of_values);
    const char *name();

};

}