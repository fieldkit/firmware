#pragma once

#include "config.h"
#include "hal/lora.h"
#include "hal/metal/rn2903.h"

#if defined(__SAMD51__)

#include <TheThingsNetwork.h>

namespace fk {

class TwoWireExtenderStream : public Stream {
private:
    Sc16is740 *bridge_;
    int32_t peek_{ -1 };

public:
    TwoWireExtenderStream(Sc16is740 &bridge);

public:
    // The number of bytes available to read.
    int available() override;
    // The first byte of incoming data available (or -1 if no data is available).
    int read() override;
    // The next byte (or character), or -1 if none is available.
    int peek() override;
    // Returns the number of bytes written.
    size_t write(uint8_t byte) override;
};

class DebugStream : public Stream {
private:
    char buffer_[256];
    size_t position_{ 0 };

public:
    DebugStream();

public:
    int available() override {
        return 0;
    }
    int read() override {
        return -1;
    }
    int peek() override {
        return -1;
    }
    size_t write(uint8_t byte) override;
};

class TheThingsLoraNetwork : public LoraNetwork {
private:
    Sc16is740 bridge_;
    TwoWireExtenderStream stream_{ bridge_ };
    DebugStream debug_;
    TheThingsNetwork ttn_{ stream_, debug_, TTN_FP_US915 };
    Availability status_{ Availability::Unknown };
    bool powered_{ false };

public:
    TheThingsLoraNetwork();

public:
    bool begin() override;
    bool stop() override;
    bool power(bool on) override;
    bool sleep(uint32_t ms) override;
    bool wake() override;
    bool factory_reset() override;
    bool configure_tx(uint8_t power_index, uint8_t data_rate) override;
    bool send_bytes(uint8_t port, uint8_t const *data, size_t size, bool confirmed) override;
    bool join(LoraOtaaJoin &otaa, int32_t retries, uint32_t retry_delay) override;
    bool join_resume() override;
    bool resume_previous_session() override;
    bool save_state() override;

    LoraErrorCode error() const override {
        return LoraErrorCode::None;
    };

    bool available() const override {
        return status_ == Availability::Available;
    }
};

class Rn2903LoraNetwork : public LoraNetwork {
private:
    Availability status_{ Availability::Unknown };
    uint8_t device_eui_[LoraDeviceEuiLength];
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
    bool factory_reset() override;
    bool configure_tx(uint8_t power_index, uint8_t data_rate) override;
    bool send_bytes(uint8_t port, uint8_t const *data, size_t size, bool confirmed) override;
    bool join(LoraOtaaJoin &otaa, int32_t retries, uint32_t retry_delay) override;
    bool join_resume() override;
    bool resume_previous_session() override;
    bool save_state() override;

public:
    Rn2903State *get_state(Pool &pool);

    LoraErrorCode error() const override {
        return rn2903_.error();
    };

    bool available() const override {
        return status_ == Availability::Available;
    }
};

} // namespace fk

#endif
