#pragma once

namespace fk {

class LoraNetwork {
public:
    virtual bool begin() = 0;

};

class NoopLoraNetwork : public LoraNetwork {
public:
    bool begin() override {
        return false;
    }
};

LoraNetwork *get_lora_network();

}

