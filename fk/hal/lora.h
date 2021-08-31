#pragma once

#include "common.h"
#include "config.h"
#include "pool.h"

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
    uint8_t power_index;
};

class LoraNetwork {
public:
    virtual bool begin() = 0;
    virtual bool stop() = 0;
    virtual bool power(bool on) = 0;
    virtual bool sleep(uint32_t seconds) = 0;
    virtual bool wake() = 0;
    virtual bool factory_reset() {
        return false;
    }
    virtual bool configure_tx(uint8_t power_index, uint8_t data_rate) {
        return false;
    }
    virtual bool send_bytes(uint8_t port, uint8_t const *data, size_t size, bool confirmed) = 0;
    virtual bool join(const char *app_eui, const char *app_key, int32_t retries = 3, uint32_t retry_delay = 10000) = 0;
    virtual bool join(const char *app_session_key, const char *network_session_key, const char *device_address, uint32_t uplink_counter,
                      uint32_t downlink_counter) = 0;
    virtual bool join_resume() {
        return false;
    }
    virtual bool resume_previous_session() = 0;
    virtual bool save_state() = 0;

public:
    virtual bool available() const = 0;
    virtual LoraErrorCode error() const = 0;
    virtual Rn2903State *get_state(Pool &pool) {
        FK_ASSERT(false);
        return nullptr;
    }
};

class NoopLoraNetwork : public LoraNetwork {
public:
    NoopLoraNetwork() {
    }

public:
    bool begin() override {
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

    bool join(const char *app_eui, const char *app_key, int32_t retries, uint32_t retry_delay) override {
        return false;
    }

    bool join(const char *app_session_key, const char *network_session_key, const char *device_address, uint32_t uplink_counter,
              uint32_t downlink_counter) override {
        return false;
    }

    bool resume_previous_session() override {
        return false;
    }

    bool save_state() override {
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
