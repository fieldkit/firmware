#include "records.h"
#include "lora_packetizer.h"
#include "clock.h"
#include "records.h"

namespace fk {

FK_DECLARE_LOGGER("packetizer");

LoraPacketizer::LoraPacketizer() {
}

LoraPacketizer::~LoraPacketizer() {
}

class LoraRecord {
private:
    static constexpr size_t MaxReadingsPerPacket = 32;
    static constexpr size_t TagSize = 1;

private:
    Pool *pool_;
    fk_data_LoraRecord *record_{ nullptr };
    float *values_{ nullptr };
    pb_array_t values_array_{ };
    uint8_t previous_sensor_{ 0 };
    size_t encoded_size_{ 0 };

public:
    explicit LoraRecord(Pool &pool) : pool_(&pool) {
        record_ = pool.malloc<fk_data_LoraRecord>();
        values_ = pool.malloc<float>(MaxReadingsPerPacket);
        clear();
    }

public:
    size_t encoded_size() const {
        return encoded_size_;
    }

public:
    void begin(uint32_t time, uint32_t reading) {
        record_->time = time;
        record_->number = reading;

        encoded_size_ = 0;
        encoded_size_ += record_->time > 0 ? pb_varint_size(record_->time) + TagSize : 0;
        encoded_size_ += record_->number > 0 ? pb_varint_size(record_->number) + TagSize : 0;
    }

    void clear() {
        fk_lora_record_encoding_new(record_);
        record_->time = 0;
        record_->number = 0;
        record_->module = 0;
        encoded_size_ = 0;
        previous_sensor_ = 0;
        values_array_ = {
            .length = 0,
            .itemSize = sizeof(float),
            .buffer = (void *)&values_,
        };

        bzero(values_, sizeof(float) * MaxReadingsPerPacket);
    }

    size_t size_of_encoding(uint8_t module, uint8_t sensor, float value) const {
        size_t size = 0u;

        if (values_array_.length == 0) {
            size += module > 0 ? pb_varint_size(module) + TagSize : 0;
            size += sensor > 0 ? pb_varint_size(sensor) + TagSize : 0;
            size += pb_varint_size(MaxReadingsPerPacket) + TagSize;
        }
        else {
            if (previous_sensor_ + 1 != sensor) {
                size += pb_varint_size(sensor) + TagSize;
            }
        }

        size += sizeof(float);

        return size;
    }

    void write_reading(uint8_t module, uint8_t sensor, float value) {
        if (values_array_.length == 0) {
            record_->module = module;
            record_->sensor = sensor;
            record_->values.arg = (void *)&values_array_;
            previous_sensor_ = sensor;
        }
        else {
            if (previous_sensor_ + 1 != sensor) {
                record_->sensor = sensor;
            }
        }

        FK_ASSERT(record_->module == module);

        values_[values_array_.length] = value;

        auto size = size_of_encoding(module, sensor, value);

        previous_sensor_ = sensor;
        encoded_size_ += size;
        values_array_.length++;
    }

    EncodedMessage *encode(Pool &pool) {
        if (values_array_.length == 0) {
            return nullptr;
        }
        auto encoded = pool.encode(fk_data_LoraRecord_fields, record_, false);
        if (encoded->size != encoded_size_) {
            logerror("packet size mismatch: %zd != %zd", encoded->size, encoded_size_);
            FK_ASSERT(encoded->size == encoded_size_);
        }
        return encoded;
    }

};

static void append(EncodedMessage **head, EncodedMessage **tail, EncodedMessage *node) {
    if (node == nullptr) {
        return;
    }

    node->link = nullptr;

    if ((*head) == nullptr) {
        (*head) = (*tail) = node;
    }
    else {
        (*tail)->link = node;
        (*tail) = node;
    }
}

tl::expected<EncodedMessage*, Error> LoraPacketizer::packetize(GlobalState const *gs, Pool &pool) {
    EncodedMessage *head = nullptr;
    EncodedMessage *tail = nullptr;

    auto attached = gs->dynamic.attached();
    if (attached == nullptr) {
        return tl::unexpected<Error>(Error::General);
    }

    LoraRecord record{ pool };

    record.begin(gs->readings.time, gs->readings.nreadings);

    for (auto &attached_module : attached->modules()) {
        auto position = attached_module.position();
        // auto meta = attached_module.meta();

        if (LoraTransmitVirtual || position != ModulePosition::Virtual) {
            for (auto &sensor : attached_module.sensors()) {
                auto reading = sensor.reading();

                auto integer_position = position.integer();
                auto sensor_index = sensor.index();
                auto adding = record.size_of_encoding(integer_position, sensor_index, reading.calibrated);
                if (record.encoded_size() + adding >= maximum_packet_size_) {
                    append(&head, &tail, record.encode(pool));
                    record.clear();
                }

                record.write_reading(integer_position, sensor_index, reading.calibrated);
                logdebug("reading: %d/%d %f (%zd)", integer_position, sensor_index, reading.calibrated, record.encoded_size());
            }

            append(&head, &tail, record.encode(pool));
            record.clear();
        }
    }

    append(&head, &tail, record.encode(pool));

    return head;
}

}
