#pragma once

#include "common.h"

namespace fk {

struct NetworkDuration {
private:
    uint32_t seconds_{ 0 };
    uint32_t on_{ 0 };

public:
    NetworkDuration();

public:
    bool always_on() const;
    bool on(uint32_t activity) const;
    NetworkDuration operator=(uint32_t seconds);

};

} // namespace fk
