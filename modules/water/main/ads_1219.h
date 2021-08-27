#pragma once

#include "common.h"
#include "hal/board.h"

namespace fk {

#define ADS1219_VREF_MASK     0xFE
#define ADS1219_VREF_INTERNAL 0x00
#define ADS1219_VREF_EXTERNAL 0x01

#define ADS1219_MUX_MASK     0x1F
#define ADS1219_MUX_DIFF_0_1 0x00
#define ADS1219_MUX_DIFF_2_3 0x20
#define ADS1219_MUX_DIFF_1_2 0x40
#define ADS1219_MUX_SINGLE_0 0x60
#define ADS1219_MUX_SINGLE_1 0x80
#define ADS1219_MUX_SINGLE_2 0xA0
#define ADS1219_MUX_SINGLE_3 0xC0
#define ADS1219_MUX_SHORTED  0xE0

#define ADS1219_GAIN_MASK 0xEF
#define ADS1219_GAIN_ONE  0x00
#define ADS1219_GAIN_FOUR 0x10

#define ADS1219_DATA_RATE_MASK 0xF3
#define ADS1219_DATA_RATE_20   0x00 // 00
#define ADS1219_DATA_RATE_90   0x04 // 01
#define ADS1219_DATA_RATE_330  0x08 // 10
#define ADS1219_DATA_RATE_1000 0x0c // 11

#define ADS1219_MODE_MASK        0xFD
#define ADS1219_MODE_SINGLE_SHOT 0x00
#define ADS1219_MODE_CONTINUOUS  0x02

enum class Ads1219VoltageReference : uint8_t { Internal = ADS1219_VREF_INTERNAL, External = ADS1219_VREF_EXTERNAL };

enum class Ads1219Gain : uint8_t {
    One = ADS1219_GAIN_ONE,
    Four = ADS1219_GAIN_FOUR,
};

enum class Ads1219Channel : uint8_t {
    Diff_0_1 = ADS1219_MUX_DIFF_0_1,
    Diff_2_3 = ADS1219_MUX_DIFF_2_3,
    Diff_1_2 = ADS1219_MUX_DIFF_1_2,
    Single_0 = ADS1219_MUX_SINGLE_0,
    Single_1 = ADS1219_MUX_SINGLE_1,
    Single_2 = ADS1219_MUX_SINGLE_2,
    Single_3 = ADS1219_MUX_SINGLE_3,
    Shorted = ADS1219_MUX_SHORTED,
};

enum class Ads1219DataRate : uint8_t {
    DataRate_20 = ADS1219_DATA_RATE_20,
    DataRate_90 = ADS1219_DATA_RATE_90,
    DataRate_330 = ADS1219_DATA_RATE_330,
    DataRate_1000 = ADS1219_DATA_RATE_1000,
};

class Ads1219ReadyChecker {
public:
    virtual bool block_until_ready(TwoWireWrapper &bus) = 0;
};

class Ads1219 {
private:
    TwoWireWrapper &bus_;
    uint8_t address_{ 0 };
    Ads1219ReadyChecker *ready_{ nullptr };
    uint8_t config_{ 0 };

public:
    Ads1219(TwoWireWrapper &bus, uint8_t address, Ads1219ReadyChecker *ready);
    virtual ~Ads1219();

public:
    bool begin();
    bool configure(Ads1219VoltageReference vref, Ads1219Channel channel, Ads1219Gain gain, Ads1219DataRate dr);
    bool read(int32_t &value);

private:
    bool read_conversion(int32_t &value);
    bool start();
};

} // namespace fk
