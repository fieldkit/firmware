#pragma once

namespace fk {

class ModMux {
public:
    virtual bool begin() = 0;
    virtual bool enable_all_modules() = 0;
    virtual bool disable_all_modules() = 0;

};

}
