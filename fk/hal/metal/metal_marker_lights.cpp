#include "hal/metal/metal_marker_lights.h"
#include "platform.h"

#include <Arduino.h>

namespace fk {

#if defined(FK_UNDERWATER)

const uint8_t MarkerLightsPower = 69u;

void MarkerLights::begin() {
    on_at_ = 0;
    pinMode(MarkerLightsPower, OUTPUT);
    digitalWrite(MarkerLightsPower, HIGH);
}

void MarkerLights::on() {
    digitalWrite(MarkerLightsPower, HIGH);
    on_at_ = fk_uptime();
}

void MarkerLights::off() {
    digitalWrite(MarkerLightsPower, LOW);
}

void MarkerLights::service() {
    if (on_at_ > 0) {
        //
    }
}

static MarkerLights marker_lights;

MarkerLights *get_marker_lights() {
    return &marker_lights;
}

#else

static NoopMarkerLights marker_lights;

NoopMarkerLights *get_marker_lights() {
    return &marker_lights;
}

#endif

} // namespace fk
