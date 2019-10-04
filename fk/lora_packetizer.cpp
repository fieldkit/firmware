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
    uint32_t encoded_size_{ 0 };

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
    uint32_t encoded_size() const {
        return encoded_size_;
    }

public:
    void begin(uint32_t time, uint32_t reading) {
        record_.time = time;
        record_.number = reading;

        encoded_size_ += pb_varint_size(record_.time) + 1;
        encoded_size_ += pb_varint_size(record_.number) + 1;
    }

    void write_reading(uint8_t module, uint8_t sensor, float value) {
        if (values_array_.length == 0) {
            record_.module = module;
            record_.sensor = sensor;
            record_.values.arg = (void *)&values_array_;
            encoded_size_ += pb_varint_size(MaxReadingsPerPacket);
            encoded_size_ += pb_varint_size(sensor);
            encoded_size_ += pb_varint_size(module);
        }
        else {
            FK_ASSERT(previous_sensor_ + 1 == sensor);
        }

        FK_ASSERT(record_.module == module);

        values_[values_array_.length++] = value;

        previous_sensor_ = sensor;
        encoded_size_ += sizeof(float);
    }

    EncodedMessage *encode(Pool &pool) {
        if (values_array_.length == 0) {
            return nullptr;
        }
        return pool.encode(fk_data_LoraRecord_fields, &record_, false);
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

tl::expected<EncodedMessage*, Error> LoraPacketizer::packetize(ModuleReadingsCollection const &readings, Pool &pool) {
    EncodedMessage *head = nullptr;
    EncodedMessage *tail = nullptr;

    LoraRecord record{ pool };

    record.begin(get_clock_now(), 323432);

    for (auto &module : readings) {
        for (auto s = 0u; s < module.readings->size(); ++s) {
            auto value = module.readings->get(s);
            record.write_reading(module.position, s, value);

            if (record.encoded_size() >= maximum_packet_size_) {
                append(&head, &tail, record.encode(pool));

                record = { pool };
            }
        }

        append(&head, &tail, record.encode(pool));

        record = { pool };
    }

    append(&head, &tail, record.encode(pool));

    return head;
}

}
