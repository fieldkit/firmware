#include "update_readings_listener.h"
#include "state_manager.h"

namespace fk {

UpdateReadingsListener::UpdateReadingsListener(Pool &pool) : pool_(pool) {
}

int32_t UpdateReadingsListener::readings_taken(state::AttachedModule *attached_module, ModuleReadings *readings, Pool *pool) {
    return 0;
}

int32_t UpdateReadingsListener::sensor_reading(state::AttachedModule *attached_module, state::AttachedSensor *sensor, SensorReading reading,
                                               Pool *pool) {
    queue_.add(sensor_reading_t{ 0, attached_module, sensor, reading });
    return 0;
}

int32_t UpdateReadingsListener::flush() {
    GlobalStateManager gsm;
    return gsm.apply([&](GlobalState *gs) -> int32_t {
        // TODO Verify same state.

        for (auto &entry : queue_) {
            entry.sensor->reading(entry.reading);
        }

        queue_ = collection<sensor_reading_t>{ pool_ };

        return 0;
    });
}

} // namespace fk
