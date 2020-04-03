#include "records.h"

namespace fk {

static inline bool pb_data_network_info_item_decode(pb_istream_t *stream, pb_array_t *array) {
    fk_data_NetworkInfo info;
    info.ssid.funcs.decode = pb_decode_string;
    info.ssid.arg = array->pool;
    info.password.funcs.decode = pb_decode_string;
    info.password.arg = array->pool;

    if (!pb_decode(stream, fk_data_NetworkInfo_fields, &info)) {
        return false;
    }

    // TODO: Wasteful.
    auto previous = (const void *)array->buffer;
    array->length++;
    array->buffer = array->pool->malloc(array->itemSize * array->length);
    void *ptr = ((uint8_t *)array->buffer) + ((array->length - 1) * array->itemSize);
    if (previous != nullptr) {
        memcpy(array->buffer, previous, ((array->length - 1) * array->itemSize));
    }
    memcpy(ptr, &info, array->itemSize);

    return true;
}

static inline bool pb_app_sensor_and_value_item_decode(pb_istream_t *stream, pb_array_t *array) {
    fk_data_SensorAndValue sensor_value = fk_data_SensorAndValue_init_default;

    if (!pb_decode(stream, fk_data_SensorAndValue_fields, &sensor_value)) {
        return false;
    }

    // TODO: Wasteful.
    auto previous = (const void *)array->buffer;
    array->length++;
    array->buffer = array->pool->malloc(array->itemSize * array->length);
    void *ptr = ((uint8_t *)array->buffer) + ((array->length - 1) * array->itemSize);
    if (previous != nullptr) {
        memcpy(array->buffer, previous, ((array->length - 1) * array->itemSize));
    }
    memcpy(ptr, &sensor_value, array->itemSize);

    return true;
}

static inline bool pb_app_sensor_group_item_decode(pb_istream_t *stream, pb_array_t *array) {
    fk_data_SensorGroup sensor_group = fk_data_SensorGroup_init_default;
    sensor_group.readings.funcs.decode = pb_decode_array;
    sensor_group.readings.arg = (void *)array->pool->malloc_with<pb_array_t>({
        .length = 0,
        .itemSize = sizeof(fk_data_SensorAndValue),
        .buffer = nullptr,
        .fields = fk_data_SensorAndValue_fields,
        .decode_item_fn = pb_app_sensor_and_value_item_decode,
        .pool = array->pool,
    });

    if (!pb_decode(stream, fk_data_SensorGroup_fields, &sensor_group)) {
        return false;
    }

    // TODO: Wasteful.
    auto previous = (const void *)array->buffer;
    array->length++;
    array->buffer = array->pool->malloc(array->itemSize * array->length);
    void *ptr = ((uint8_t *)array->buffer) + ((array->length - 1) * array->itemSize);
    if (previous != nullptr) {
        memcpy(array->buffer, previous, ((array->length - 1) * array->itemSize));
    }
    memcpy(ptr, &sensor_group, array->itemSize);

    return true;
}

static inline bool pb_app_network_info_item_decode(pb_istream_t *stream, pb_array_t *array) {
    fk_app_NetworkInfo info;
    info.ssid.funcs.decode = pb_decode_string;
    info.ssid.arg = array->pool;
    info.password.funcs.decode = pb_decode_string;
    info.password.arg = array->pool;

    if (!pb_decode(stream, fk_app_NetworkInfo_fields, &info)) {
        return false;
    }

    // TODO: Wasteful.
    auto previous = (const void *)array->buffer;
    array->length++;
    array->buffer = array->pool->malloc(array->itemSize * array->length);
    void *ptr = ((uint8_t *)array->buffer) + ((array->length - 1) * array->itemSize);
    if (previous != nullptr) {
        memcpy(array->buffer, previous, ((array->length - 1) * array->itemSize));
    }
    memcpy(ptr, &info, array->itemSize);

    return true;
}

fk_data_DataRecord fk_data_record_decoding_readings_new(Pool &pool) {
    fk_data_DataRecord record = fk_data_DataRecord_init_default;
    record.readings.sensorGroups.funcs.decode = pb_decode_array;
    record.readings.sensorGroups.arg = (void *)pool.malloc_with<pb_array_t>({
        .length = 0,
        .itemSize = sizeof(fk_data_SensorGroup),
        .buffer = nullptr,
        .fields = fk_data_SensorGroup_fields,
        .decode_item_fn = pb_app_sensor_group_item_decode,
        .pool = &pool,
    });
    return record;
}

fk_data_DataRecord fk_data_record_decoding_new(Pool &pool) {
    fk_data_DataRecord record = fk_data_DataRecord_init_default;
    record.metadata.firmware.version.funcs.decode = pb_decode_string;
    record.metadata.firmware.version.arg = (void *)&pool;
    record.metadata.firmware.build.funcs.decode = pb_decode_string;
    record.metadata.firmware.build.arg = (void *)&pool;
    record.metadata.firmware.hash.funcs.decode = pb_decode_string;
    record.metadata.firmware.hash.arg = (void *)&pool;
    record.metadata.firmware.number.funcs.decode = pb_decode_string;
    record.metadata.firmware.number.arg = (void *)&pool;
    record.metadata.deviceId.funcs.decode = pb_decode_data;
    record.metadata.deviceId.arg = (void *)&pool;
    record.metadata.generation.funcs.decode = pb_decode_data;
    record.metadata.generation.arg = (void *)&pool;
    record.identity.name.funcs.decode = pb_decode_string;
    record.identity.name.arg = (void *)&pool;
    record.lora.appEui.funcs.decode = pb_decode_data;
    record.lora.appEui.arg = (void *)&pool;
    record.lora.appKey.funcs.decode = pb_decode_data;
    record.lora.appKey.arg = (void *)&pool;
    record.lora.deviceEui.funcs.decode = pb_decode_data;
    record.lora.deviceEui.arg = (void *)&pool;
    record.lora.appSessionKey.funcs.decode = pb_decode_data;
    record.lora.appSessionKey.arg = (void *)&pool;
    record.lora.networkSessionKey.funcs.decode = pb_decode_data;
    record.lora.networkSessionKey.arg = (void *)&pool;
    record.lora.deviceAddress.funcs.decode = pb_decode_data;
    record.lora.deviceAddress.arg = (void *)&pool;
    record.network.networks.funcs.decode = pb_decode_array;
    record.network.networks.arg = (void *)pool.malloc_with<pb_array_t>({
        .length = 0,
        .itemSize = sizeof(fk_data_NetworkInfo),
        .buffer = nullptr,
        .fields = fk_data_NetworkInfo_fields,
        .decode_item_fn = pb_data_network_info_item_decode,
        .pool = &pool,
    });

    record.schedule.readings.cron.funcs.decode = pb_decode_data;
    record.schedule.readings.cron.arg = (void *)&pool;
    record.schedule.network.cron.funcs.decode = pb_decode_data;
    record.schedule.network.cron.arg = (void *)&pool;
    record.schedule.gps.cron.funcs.decode = pb_decode_data;
    record.schedule.gps.cron.arg = (void *)&pool;
    record.schedule.lora.cron.funcs.decode = pb_decode_data;
    record.schedule.lora.cron.arg = (void *)&pool;
    record.transmission.wifi.url.funcs.decode = pb_decode_string;
    record.transmission.wifi.url.arg = (void *)&pool;
    record.transmission.wifi.token.funcs.decode = pb_decode_string;
    record.transmission.wifi.token.arg = (void *)&pool;

    return record;
}

fk_data_DataRecord fk_data_record_encoding_new() {
    fk_data_DataRecord record = fk_data_DataRecord_init_default;
    record.metadata.firmware.version.funcs.encode = pb_encode_string;
    record.metadata.firmware.build.funcs.encode = pb_encode_string;
    record.metadata.firmware.number.funcs.encode = pb_encode_string;
    record.metadata.firmware.hash.funcs.encode = pb_encode_string;
    record.metadata.deviceId.funcs.encode = pb_encode_data;
    record.metadata.generation.funcs.encode = pb_encode_data;
    record.identity.name.funcs.encode = pb_encode_string;
    record.readings.sensorGroups.funcs.encode = pb_encode_array;
    record.modules.funcs.encode = pb_encode_array;
    record.lora.appEui.funcs.encode = pb_encode_data;
    record.lora.appKey.funcs.encode = pb_encode_data;
    record.lora.appSessionKey.funcs.encode = pb_encode_data;
    record.lora.networkSessionKey.funcs.encode = pb_encode_data;
    record.lora.deviceAddress.funcs.encode = pb_encode_data;
    record.lora.deviceEui.funcs.encode = pb_encode_data;
    record.network.networks.funcs.encode = pb_encode_array;
    record.schedule.readings.cron.funcs.encode = pb_encode_data;
    record.schedule.network.cron.funcs.encode = pb_encode_data;
    record.schedule.gps.cron.funcs.encode = pb_encode_data;
    record.schedule.lora.cron.funcs.encode = pb_encode_data;
    record.transmission.wifi.url.funcs.encode = pb_encode_string;
    record.transmission.wifi.token.funcs.encode = pb_encode_string;

    return record;
}

fk_app_HttpReply fk_http_reply_encoding() {
    return { };
}

fk_app_HttpQuery *fk_http_query_prepare_decoding(fk_app_HttpQuery *query, Pool *pool) {
    *query = fk_app_HttpQuery_init_default;

    query->identity.name.funcs.decode = pb_decode_string;
    query->identity.name.arg = (void *)pool;

    query->schedules.readings.cron.funcs.decode = pb_decode_data;
    query->schedules.readings.cron.arg = (void *)pool;

    query->loraSettings.deviceEui.funcs.decode = pb_decode_data;
    query->loraSettings.deviceEui.arg = (void *)pool;

    query->loraSettings.appEui.funcs.decode = pb_decode_data;
    query->loraSettings.appEui.arg = (void *)pool;

    query->loraSettings.appKey.funcs.decode = pb_decode_data;
    query->loraSettings.appKey.arg = (void *)pool;

    query->loraSettings.appSessionKey.funcs.decode = pb_decode_data;
    query->loraSettings.appSessionKey.arg = (void *)pool;

    query->loraSettings.networkSessionKey.funcs.decode = pb_decode_data;
    query->loraSettings.networkSessionKey.arg = (void *)pool;

    query->loraSettings.deviceAddress.funcs.decode = pb_decode_data;
    query->loraSettings.deviceAddress.arg = (void *)pool;

    query->networkSettings.networks.funcs.decode = pb_decode_array;
    query->networkSettings.networks.arg = (void *)pool->malloc_with<pb_array_t>({
        .length = 0,
        .itemSize = sizeof(fk_app_NetworkInfo),
        .buffer = nullptr,
        .fields = fk_app_NetworkInfo_fields,
        .decode_item_fn = pb_app_network_info_item_decode,
        .pool = pool,
    });

    query->transmission.wifi.url.funcs.decode = pb_decode_string;
    query->transmission.wifi.url.arg = (void *)pool;

    query->transmission.wifi.token.funcs.decode = pb_decode_string;
    query->transmission.wifi.token.arg = (void *)pool;

    return query;
}

fk_app_HttpReply *fk_http_reply_encoding_initialize(fk_app_HttpReply *reply) {
    if (reply->errors.arg != nullptr) {
        reply->errors.funcs.encode = pb_encode_array;
        auto array = (pb_array_t *)reply->errors.arg;
        for (size_t i = 0; i < array->length; ++i) {
            auto error = &((fk_app_Error *)array->buffer)[i];
            error->message.funcs.encode = pb_encode_string;
        }
    }

    if (reply->status.identity.device.arg != nullptr) reply->status.identity.device.funcs.encode = pb_encode_string;
    if (reply->status.identity.stream.arg != nullptr) reply->status.identity.stream.funcs.encode = pb_encode_string;
    if (reply->status.identity.deviceId.arg != nullptr) reply->status.identity.deviceId.funcs.encode = pb_encode_data;
    if (reply->status.identity.firmware.arg != nullptr) reply->status.identity.firmware.funcs.encode = pb_encode_string;
    if (reply->status.identity.build.arg != nullptr) reply->status.identity.build.funcs.encode = pb_encode_string;
    if (reply->status.identity.number.arg != nullptr) reply->status.identity.number.funcs.encode = pb_encode_string;
    if (reply->status.identity.name.arg != nullptr) reply->status.identity.name.funcs.encode = pb_encode_string;
    if (reply->status.identity.generation.arg != nullptr) reply->status.identity.generation.funcs.encode = pb_encode_data;

    if (reply->status.firmware.version.arg != nullptr) reply->status.firmware.version.funcs.encode = pb_encode_string;
    if (reply->status.firmware.build.arg != nullptr) reply->status.firmware.build.funcs.encode = pb_encode_string;
    if (reply->status.firmware.number.arg != nullptr) reply->status.firmware.number.funcs.encode = pb_encode_string;
    if (reply->status.firmware.hash.arg != nullptr) reply->status.firmware.hash.funcs.encode = pb_encode_string;

    if (reply->modules.arg != nullptr) {
        reply->modules.funcs.encode = pb_encode_array;
        auto array = (pb_array_t *)reply->modules.arg;
        for (size_t i = 0; i < array->length; ++i) {
            auto module = &((fk_app_ModuleCapabilities *)array->buffer)[i];
            module->name.funcs.encode = pb_encode_string;
            module->path.funcs.encode = pb_encode_string;
            module->id.funcs.encode = pb_encode_data;
            if (module->sensors.arg != nullptr) {
                module->sensors.funcs.encode = pb_encode_array;
                auto array = (pb_array_t *)module->sensors.arg;
                for (size_t i = 0; i < array->length; ++i) {
                    auto sensor = &((fk_app_SensorCapabilities *)array->buffer)[i];
                    sensor->name.funcs.encode = pb_encode_string;
                    sensor->path.funcs.encode = pb_encode_string;
                    sensor->unitOfMeasure.funcs.encode = pb_encode_string;
                }
            }
        }
    }

    if (reply->streams.arg != nullptr) {
        reply->streams.funcs.encode = pb_encode_array;
        auto array = (pb_array_t *)reply->streams.arg;
        for (size_t i = 0; i < array->length; ++i) {
            auto stream = &((fk_app_DataStream *)array->buffer)[i];
            stream->name.funcs.encode = pb_encode_string;
            stream->path.funcs.encode = pb_encode_string;
            stream->hash.funcs.encode = pb_encode_data;
        }
    }

    if (reply->networkSettings.networks.arg != nullptr) {
        reply->networkSettings.networks.funcs.encode = pb_encode_array;
        auto array = (pb_array_t *)reply->networkSettings.networks.arg;
        for (size_t i = 0; i < array->length; ++i) {
            auto network = &((fk_app_NetworkInfo *)array->buffer)[i];
            network->ssid.funcs.encode = pb_encode_string;
            network->password.funcs.encode = pb_encode_string;
        }
    }

    if (reply->liveReadings.arg != nullptr) {
        reply->liveReadings.funcs.encode = pb_encode_array;
        auto live_readings_array = (pb_array_t *)reply->liveReadings.arg;
        for (size_t i = 0; i < live_readings_array->length; ++i) {
            auto live_readings = &((fk_app_LiveReadings *)live_readings_array->buffer)[i];
            if (live_readings->modules.arg != nullptr) {
                live_readings->modules.funcs.encode = pb_encode_array;
                auto modules_array = (pb_array_t *)live_readings->modules.arg;
                for (size_t j = 0; j < modules_array->length; ++j) {
                    auto lmr = &((fk_app_LiveModuleReadings *)modules_array->buffer)[j];
                    if (lmr->module.name.arg != nullptr) {
                        lmr->module.name.funcs.encode = pb_encode_string;
                        lmr->module.id.funcs.encode = pb_encode_data;
                    }
                    if (lmr->readings.arg != nullptr) {
                        lmr->readings.funcs.encode = pb_encode_array;
                        auto readings_array = (pb_array_t *)lmr->readings.arg;
                        for (size_t k = 0; k < readings_array->length; ++k) {
                            auto lsr = &((fk_app_LiveSensorReading *)readings_array->buffer)[k];
                            lsr->sensor.name.funcs.encode = pb_encode_string;
                            lsr->sensor.unitOfMeasure.funcs.encode = pb_encode_string;
                        }
                    }
                }
            }
        }
    }

    if (reply->transmission.wifi.url.arg != nullptr) {
        reply->transmission.wifi.url.funcs.encode = pb_encode_string;
    }

    if (reply->transmission.wifi.token.arg != nullptr) {
        reply->transmission.wifi.token.funcs.encode = pb_encode_string;
    }

    return reply;
}

fk_data_LoraRecord fk_lora_record_encoding_new() {
    fk_data_LoraRecord record = fk_data_LoraRecord_init_default;

    record.values.funcs.encode = pb_encode_array;
    record.deviceId.funcs.encode = pb_encode_data;

    return record;
}

}
