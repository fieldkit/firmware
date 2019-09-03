#pragma once

#include "common.h"

namespace fk {

class LoraNetwork {
public:
    virtual bool begin() = 0;
    virtual bool power(bool on) = 0;
    virtual bool sleep(uint32_t seconds) = 0;
    virtual bool send_bytes(uint8_t const *data, size_t size) = 0;

};

class NoopLoraNetwork : public LoraNetwork {
public:
    bool begin() override {
        return false;
    }

    bool power(bool on) override {
        return false;
    }

    bool sleep(uint32_t ms) override {
        return false;
    }

    bool send_bytes(uint8_t const *data, size_t size) override {
        return false;
    }

};

LoraNetwork *get_lora_network();

}

