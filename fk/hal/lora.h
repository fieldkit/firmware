#pragma once

#include "common.h"
#include "config.h"
#include "pool.h"
#include "lora_frequency.h"

namespace fk {

enum class LoraErrorCode {
    None,
    ModuleIO,
    NotJoined,
    DataLength,
    KeysNotInitialized,
    Mac,
};

struct Rn2903State {
    uint8_t device_eui[LoraDeviceEuiLength];
    uint8_t app_key[LoraAppKeyLength];
    uint8_t join_eui[LoraJoinEuiLength];
    uint8_t device_address[LoraDeviceAddressLength];
    uint32_t uplink_counter;
    uint32_t downlink_counter;
    uint32_t rx_delay_1;
    uint32_t rx_delay_2;
    uint8_t power_index;
};

struct LoraOtaaJoin {
    uint32_t frequency;
    const char *device_eui;
    const char *app_key;
    const char *join_eui;
};

class LoraNetwork {
public:
    virtual bool begin(lora_frequency_t frequency_band) = 0;
    virtual bool stop() = 0;
    virtual bool power(bool on) = 0;
    virtual bool sleep(uint32_t seconds) = 0;
    virtual bool wake() = 0;
    virtual bool factory_reset() {
        return false;
    }
    virtual bool send_bytes(uint8_t port, uint8_t const *data, size_t size, bool confirmed) = 0;
    virtual bool join(LoraOtaaJoin &otaa, int32_t retries = 3, uint32_t retry_delay = 10000) = 0;
    virtual bool set_rx_delays(uint32_t rx1) {
        return false;
    }
    virtual bool join_resume() {
        return false;
    }
    virtual bool save_state() {
        return false;
    }

public:
    virtual bool available() const = 0;
    virtual LoraErrorCode error() const = 0;
    virtual bool get_state(Rn2903State *state) {
        FK_ASSERT(false);
        return false;
    }
    virtual Rn2903State *get_state(Pool &pool) {
        FK_ASSERT(false);
        return nullptr;
    }
    virtual bool awake() const {
        return false;
    }
};

class NoopLoraNetwork : public LoraNetwork {
public:
    NoopLoraNetwork() {
    }

public:
    bool begin(lora_frequency_t frequency_band) override {
        return false;
    }

    bool stop() override {
        return false;
    }

    bool power(bool on) override {
        return false;
    }

    bool sleep(uint32_t ms) override {
        return false;
    }

    bool wake() override {
        return false;
    }

    bool send_bytes(uint8_t port, uint8_t const *data, size_t size, bool confirmed) override {
        return false;
    }

    bool join(LoraOtaaJoin &otaa, int32_t retries = 3, uint32_t retry_delay = 10000) override {
        return false;
    }

    bool available() const override {
        return false;
    }

    LoraErrorCode error() const override {
        return LoraErrorCode::None;
    }
};

LoraNetwork *get_lora_network();

} // namespace fk
