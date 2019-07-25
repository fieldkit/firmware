#pragma once

namespace fk {

class ModMux {
protected:
    bool available_;

public:
    virtual bool begin() = 0;
    virtual bool enable_all_modules() = 0;
    virtual bool disable_all_modules() = 0;
    virtual bool choose(uint8_t position) = 0;

public:
    bool available() const {
        return available_;
    }

};

ModMux *get_modmux();

}
