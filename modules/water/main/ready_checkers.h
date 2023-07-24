#pragma once

#include "common.h"
#include "platform.h"
#include "mcp_2803.h"
#include "ads_1219.h"

namespace fk {

class NoopReadyChecker : public Ads1219ReadyChecker {
public:
    bool block_until_ready(TwoWireWrapper &bus) override {
        return true;
    }
};

class Ads1219ReadyAfterDelay : public Ads1219ReadyChecker {
private:
    uint32_t delay_{ 1 };

public:
    Ads1219ReadyAfterDelay() {
    }

public:
    bool block_until_ready(TwoWireWrapper &bus) override {
        fk_delay(delay_);
        return true;
    }
};

class UnexciteBeforeReadyChecker : public Ads1219ReadyChecker {
private:
    Mcp2803 &mcp2803_;
    Mcp2803Config config_;

public:
    UnexciteBeforeReadyChecker(Mcp2803 &mcp2803, Mcp2803Config config) : mcp2803_(mcp2803), config_(config) {
    }

public:
    bool block_until_ready(TwoWireWrapper &bus) override {
        if (!mcp2803_.configure(config_)) {
            return false;
        }
        return true;
    }
};

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

            fk_delay(20);
        }
        return false;
    }
};

}