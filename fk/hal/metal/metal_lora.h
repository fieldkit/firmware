#pragma once

#include "hal/lora.h"
#include "hal/metal/rn2903.h"
#include "config.h"

#if defined(ARDUINO)

namespace fk {

class Rn2903LoraNetwork : public LoraNetwork {
private:
    Availability status_{ Availability::Unknown };
    uint8_t device_eui_[LoraDeviceEuiLength];
    uint32_t uplink_counter_{ 0 };
    bool powered_{ false };
    Rn2903 rn2903_;

public:
    Rn2903LoraNetwork();

public:
    bool begin() override;
    bool stop() override;
    bool power(bool on) override;
    bool sleep(uint32_t ms) override;
    bool wake() override;
    bool send_bytes(uint8_t port, uint8_t const *data, size_t size, bool confirmed) override;
    bool join(const char *app_eui, const char *app_key, int32_t retries = 3, uint32_t retry_delay = 10000) override;
    bool join(const char *app_session_key, const char *network_session_key, const char *device_address, uint32_t uplink_counter, uint32_t downlink_counter) override;
    bool resume_previous_session() override;
    bool save_state() override;

public:
    uint32_t uplink_counter() override {
        return uplink_counter_;
    }

    uint8_t const *device_eui() const override {
        return device_eui_;
    }

    LoraErrorCode error() const override {
        return rn2903_.error();
    };

    bool available() const override {
        return status_ == Availability::Available;
    }

public:
    bool query_status();

private:
    bool update_uplink_counter();

};

}

#endif
