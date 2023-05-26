#pragma once

#include <modules/bridge/modules.h>

#include "common.h"
#include "pool.h"
#include "curves.h"
#include "ads_1219.h"
#include "mcp_2803.h"

namespace fk {

enum WaterModality { Temp, PH, EC, DO, ORP };

#define FK_MCP2803_ADDRESS 0x22
#define FK_ADS1219_ADDRESS 0x45

struct WaterMcpGpioConfig {
    uint8_t io_dir;
    uint8_t pullups;
    uint8_t on;
    uint8_t off;
    uint8_t excite_on;
    uint8_t excite_off;
};

struct WaterReadings {
    float uncalibrated;
    float calibrated;
    float factory;
};

class WaterProtocol : public Ads1219ReadyChecker {
private:
    Pool &pool_;
    TwoWireWrapper &bus_;
    WaterModality modality_;
    WaterMcpGpioConfig mcp_config_;
    bool standalone_orp_{ false };
    Mcp2803 mcp_{ bus_, FK_MCP2803_ADDRESS };
    Ads1219 ads_{ bus_, FK_ADS1219_ADDRESS, this };
    Ads1219ReadyChecker *readings_checker_{ nullptr };

public:
    WaterProtocol(Pool &pool, TwoWireWrapper &bus, WaterModality modality, WaterMcpGpioConfig mcp_config, bool standalone_orp);

private:
    bool excite_control(bool high);
    Curve *create_modules_default_curve(Pool &pool);
    bool excite_enabled() const;
    bool averaging_enabled() const;

public:
    bool initialize();

    WaterReadings *take_readings(ReadingsContext mc, calibration_config_t *cal, Pool &pool);

    bool block_until_ready(TwoWireWrapper &bus) override;

    bool lockout_enabled() const;
};

} // namespace fk