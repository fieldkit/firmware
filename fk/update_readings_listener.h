#pragma once

#include "state/modules.h"

namespace fk {

class UpdateReadingsListener : public state::ReadingsListener {
private:
    struct sensor_reading_t {
        uint32_t version;
        state::AttachedModule *attached_module;
        state::AttachedSensor *sensor;
        SensorReading reading;
    };

    Pool &pool_;
    collection<sensor_reading_t> queue_{ pool_ };

public:
    UpdateReadingsListener(Pool &pool);

public:
    int32_t readings_taken(state::AttachedModule *attached_module, ModuleReadings *readings, Pool *pool) override;
    int32_t sensor_reading(state::AttachedModule *attached_module, state::AttachedSensor *sensor, SensorReading reading,
                           Pool *pool) override;

public:
    int32_t flush();
};

} // namespace fk
