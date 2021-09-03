#include "lora_packetizer.h"
#include "clock.h"
#include "records.h"

namespace fk {

FK_DECLARE_LOGGER("packetizer");

static void append(EncodedMessage **head, EncodedMessage **tail, EncodedMessage *node);

struct SensorTemplate {
    uint32_t manufacturer;
    uint32_t kind;
    uint32_t sensor_index;
};

struct SensorGroupTemplate {
    collection<SensorTemplate> sensors;

    SensorGroupTemplate(Pool &pool) : sensors{ pool } {
    }
};

static tl::expected<SensorGroupTemplate *, Error> get_sensor_group_template(GlobalState const *gs, Pool &pool) {
    auto attached = gs->dynamic.attached();
    if (attached == nullptr) {
        return nullptr;
    }

    for (auto &attached_module : attached->modules()) {
        auto meta = attached_module.meta();
        if (meta->manufacturer == FK_MODULES_MANUFACTURER && meta->kind == FK_MODULES_KIND_WEATHER) {
            loginfo("found sensor group: weather");

            auto sensor_group = new (pool) SensorGroupTemplate(pool);
            // Temperature, pressure, humidity, and rain.
            sensor_group->sensors.add(SensorTemplate{ meta->manufacturer, meta->kind, 0 });
            sensor_group->sensors.add(SensorTemplate{ meta->manufacturer, meta->kind, 1 });
            sensor_group->sensors.add(SensorTemplate{ meta->manufacturer, meta->kind, 2 });
            sensor_group->sensors.add(SensorTemplate{ meta->manufacturer, meta->kind, 4 });
            // Battery level.
            sensor_group->sensors.add(SensorTemplate{ FK_MODULES_MANUFACTURER, FK_MODULES_KIND_DIAGNOSTICS, 0 });
            return sensor_group;
        }
    }

#if defined(FK_TESTING_DIAGNOSTICS_SENSOR_GROUP)
    auto sensor_group = new (pool) SensorGroupTemplate(pool);
    sensor_group->sensors.add(SensorTemplate{ FK_MODULES_MANUFACTURER, FK_MODULES_KIND_DIAGNOSTICS, 0 });
    sensor_group->sensors.add(SensorTemplate{ FK_MODULES_MANUFACTURER, FK_MODULES_KIND_DIAGNOSTICS, 1 });
    sensor_group->sensors.add(SensorTemplate{ FK_MODULES_MANUFACTURER, FK_MODULES_KIND_DIAGNOSTICS, 5 });
    sensor_group->sensors.add(SensorTemplate{ FK_MODULES_MANUFACTURER, FK_MODULES_KIND_DIAGNOSTICS, 10 });
    sensor_group->sensors.add(SensorTemplate{ FK_MODULES_MANUFACTURER, FK_MODULES_KIND_DIAGNOSTICS, 11 });
    return sensor_group;
#endif

    return nullptr;
}

class LoraRecord {
private:
    static constexpr size_t MaxReadingsPerPacket = 32;

private:
    Pool *pool_;
    float *values_{ nullptr };
    size_t readings_encoded_{ 0 };
    size_t encoded_size_{ 0 };

public:
    explicit LoraRecord(Pool &pool) : pool_(&pool) {
        values_ = pool.malloc<float>(MaxReadingsPerPacket);
        clear();
    }

public:
    size_t encoded_size() const {
        return encoded_size_;
    }

public:
    void begin(uint32_t time) {
        encoded_size_ = 0;
    }

    void clear() {
        encoded_size_ = 0;
        readings_encoded_ = 0;
        bzero(values_, sizeof(float) * MaxReadingsPerPacket);
    }

    size_t size_of_encoding(float value) const {
        return sizeof(float);
    }

    void write_reading(float value) {
        auto size = size_of_encoding(value);
        values_[readings_encoded_] = value;
        encoded_size_ += size;
        readings_encoded_++;
    }

    EncodedMessage *encode(Pool &pool) {
        if (readings_encoded_ == 0) {
            return nullptr;
        }
        return pool.wrap_copy((uint8_t *)values_, encoded_size_);
    }
};

LoraPacketizer::LoraPacketizer() {
}

LoraPacketizer::~LoraPacketizer() {
}

tl::expected<EncodedMessage *, Error> LoraPacketizer::packetize(GlobalState const *gs, Pool &pool) {
    EncodedMessage *head = nullptr;
    EncodedMessage *tail = nullptr;

    if (gs->readings.time > 0) {
        logwarn("no reading");
        return nullptr;
    }

    auto attached = gs->dynamic.attached();
    if (attached == nullptr) {
        logwarn("no modules attached");
        return nullptr;
    }

    auto maybe_sensor_group = get_sensor_group_template(gs, pool);
    if (!maybe_sensor_group) {
        logerror("no sensor group");
        return nullptr;
    }

    auto sensor_group = (*maybe_sensor_group);
    if (sensor_group == nullptr) {
        logwarn("no sensor group");
        return nullptr;
    }

    LoraRecord record{ pool };

    // TODO Varint of delta between the reading and the time.
    record.begin(gs->readings.time);

    for (auto &sensor_template : sensor_group->sensors) {
        for (auto &attached_module : attached->modules()) {
            auto header = attached_module.header();
            if (header.manufacturer == sensor_template.manufacturer && header.kind == sensor_template.kind) {
                for (auto &attached_sensor : attached_module.sensors()) {
                    if (attached_sensor.index() == sensor_template.sensor_index) {
                        auto reading = attached_sensor.reading();
                        auto adding = record.size_of_encoding(reading.calibrated);
                        if (record.encoded_size() + adding >= maximum_packet_size_) {
                            append(&head, &tail, record.encode(pool));
                            record.clear();
                        }
                        record.write_reading(reading.calibrated);
                        logdebug("reading: '%s.%s' %f (%zd)", attached_module.name(), attached_sensor.name(), reading.calibrated,
                                 record.encoded_size());
                    }
                }
            }
        }
    }

    append(&head, &tail, record.encode(pool));

    return head;
}

static void append(EncodedMessage **head, EncodedMessage **tail, EncodedMessage *node) {
    if (node == nullptr) {
        return;
    }

    node->link = nullptr;

    logdebug("packet: size=%d", node->size);

    if ((*head) == nullptr) {
        (*head) = (*tail) = node;
    } else {
        (*tail)->link = node;
        (*tail) = node;
    }
}

} // namespace fk
