#pragma once

#include "common.h"
#include "hal/board.h"

namespace fk {

#define ADS1219_VREF_MASK     0xFE
#define ADS1219_VREF_INTERNAL 0x00
#define ADS1219_VREF_EXTERNAL 0x01

enum class Ads1219VoltageReference {
    INTERNAL = ADS1219_VREF_INTERNAL,
    EXTERNAL = ADS1219_VREF_EXTERNAL
};

class Ads1219ReadyChecker {
public:
    virtual bool block_until_ready(TwoWireWrapper &bus) = 0;
};

class Ads1219 {
private:
    uint8_t address_{ 0 };
    Ads1219ReadyChecker *ready_{ nullptr };
    uint8_t config_{ 0 };

public:
    Ads1219(uint8_t address, Ads1219ReadyChecker *ready);
    virtual ~Ads1219();

public:
    bool begin(TwoWireWrapper &bus);
    bool set_voltage_reference(TwoWireWrapper &bus, Ads1219VoltageReference vref);
    bool read_single_ended(TwoWireWrapper &bus, int32_t channel, int32_t &value);
    bool read_differential_0_1(TwoWireWrapper &bus, int32_t &value);
    bool read_differential_2_3(TwoWireWrapper &bus, int32_t &value);
    bool read_dfferential_1_2(TwoWireWrapper &bus, int32_t &value);

private:
    bool read_dfferential(TwoWireWrapper &bus, uint8_t config, int32_t &value);
    bool read_conversion(TwoWireWrapper &bus, int32_t &value);
    bool start(TwoWireWrapper &bus);

};

} // namespace fk
