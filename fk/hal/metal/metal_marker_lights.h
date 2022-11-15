#pragma once

#include "common.h"

namespace fk {

#if defined(FK_UNDERWATER)

class MarkerLights {
private:
    uint32_t on_at_{ 0 };

public:
    void begin();
    void on();
    void off();
    void service();
};

MarkerLights *get_marker_lights();

#else

class NoopMarkerLights {
public:
    void begin() {
    }
    void on() {
    }
    void off() {
    }
    void service() {
    }
};

NoopMarkerLights *get_marker_lights();

#endif

} // namespace fk