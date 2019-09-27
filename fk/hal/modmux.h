#pragma once

namespace fk {

class ModMux {
public:
    constexpr static uint8_t VirtualPosition = 0xff;
    constexpr static uint8_t NoModuleSelected = 0xff;


protected:
    bool available_;

public:
    virtual bool begin() = 0;
    virtual bool enable_all_modules() = 0;
    virtual bool disable_all_modules() = 0;
    virtual bool enable_module(uint8_t position) = 0;
    virtual bool disable_module(uint8_t position) = 0;
    virtual bool power_cycle(uint8_t position) = 0;
    virtual bool choose(uint8_t position) = 0;
    virtual bool choose_nothing() = 0;

public:
    bool available() const {
        return available_;
    }

};

class DisableModuleOnReturn {
private:
    uint8_t position_;

public:
    DisableModuleOnReturn(uint8_t position); 
    virtual ~DisableModuleOnReturn();
};

ModMux *get_modmux();

}
