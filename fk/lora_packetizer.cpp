#include "records.h"
#include "lora_packetizer.h"
#include "clock.h"

namespace fk {

FK_DECLARE_LOGGER("packetizer");

LoraPacketizer::LoraPacketizer() {
}

LoraPacketizer::~LoraPacketizer() {
}

class LoraRecord {
private:
    static constexpr size_t MaxReadingsPerPacket = 32;

private:
    Pool *pool_;
    fk_data_LoraRecord record_;
    float values_[MaxReadingsPerPacket];
    pb_array_t values_array_;
    uint8_t previous_sensor_{ 0 };
    size_t encoded_size_{ 0 };

public:
    LoraRecord(Pool &pool) : pool_(&pool) {
        record_ = fk_data_LoraRecord_init_default;
        record_.values.funcs.encode = pb_encode_array;
        record_.time = 0;
        record_.number = 0;
        record_.module = 0;

        values_array_ = {
            .length = 0,
            .itemSize = sizeof(float),
            .buffer = (void *)&values_,
        };
    }

public:
    size_t encoded_size() const {
        return encoded_size_;
    }

public:
    void begin(uint32_t time, uint32_t reading) {
        record_.time = time;
        record_.number = reading;

        encoded_size_ += pb_varint_size(record_.time) + 1;
        encoded_size_ += pb_varint_size(record_.number) + 1;
    }

    size_t size_of_encoding(uint8_t module, uint8_t sensor, float value) const {
        auto size = 0u;

        if (values_array_.length == 0) {
            size += pb_varint_size(MaxReadingsPerPacket);
            size += sensor > 0 ? pb_varint_size(sensor) + 1 : 0;
            size += module > 0 ? pb_varint_size(module) + 1 : 0;
            size += 1; // Tag for values.
        }
        else {
            if (previous_sensor_ + 1 != sensor) {
                size += sensor > 0 ? pb_varint_size(sensor) + 1 : 0;
            }
        }

        size += sizeof(float);

        return size;
    }

    void write_reading(uint8_t module, uint8_t sensor, float value) {
        if (values_array_.length == 0) {
            record_.module = module;
            record_.sensor = sensor;
            record_.values.arg = (void *)&values_array_;
        }
        else {
            if (previous_sensor_ + 1 != sensor) {
                record_.sensor = sensor;
            }
        }

        FK_ASSERT(record_.module == module);

        values_[values_array_.length++] = value;

        previous_sensor_ = sensor;

        encoded_size_ += size_of_encoding(module, sensor, value);
    }

    EncodedMessage *encode(Pool &pool) {
        if (values_array_.length == 0) {
            return nullptr;
        }
        auto encoded = pool.encode(fk_data_LoraRecord_fields, &record_, false);
        if (encoded->size != encoded_size_) {
            logwarn("encoded size differs from predicted (%zd != %zd)", encoded->size, encoded_size_);
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

tl::expected<EncodedMessage*, Error> LoraPacketizer::packetize(TakenReadings const &taken, Pool &pool) {
    EncodedMessage *head = nullptr;
    EncodedMessage *tail = nullptr;

    LoraRecord record{ pool };

    logdebug("begin time=%" PRIu32 " reading=%" PRIu32, taken.time, taken.number);

    record.begin(taken.time, taken.number);

    for (auto &module : taken.readings) {
        if (module.position != ModMux::VirtualPosition) {
            for (auto s = 0u; s < module.readings->size(); ++s) {
                auto value = module.readings->get(s);
                if (value != 0.0f) {
                    if (record.encoded_size() + record.size_of_encoding(module.position, s, value) >= maximum_packet_size_) {
                        append(&head, &tail, record.encode(pool));

                        record = { pool };
                    }

                    logdebug("reading: %d/%d %f", module.position, s, value);
                    record.write_reading(module.position, s, value);
                }
            }

            append(&head, &tail, record.encode(pool));

            record = { pool };
        }
    }

    append(&head, &tail, record.encode(pool));

    return head;
}

}
