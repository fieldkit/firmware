#include "records.h"

namespace fk {

fk_data_DataRecord fk_data_record_decoding_new(Pool &pool) {
    fk_data_DataRecord record = fk_data_DataRecord_init_default;
    record.metadata.firmware.git.funcs.decode = pb_decode_string;
    record.metadata.firmware.git.arg = (void *)&pool;
    record.metadata.firmware.build.funcs.decode = pb_decode_string;
    record.metadata.firmware.build.arg = (void *)&pool;
    record.metadata.deviceId.funcs.decode = pb_decode_data;
    record.metadata.deviceId.arg = (void *)&pool;
    record.metadata.generation.funcs.decode = pb_decode_data;
    record.metadata.generation.arg = (void *)&pool;
    record.identity.name.funcs.decode = pb_decode_string;
    record.identity.name.arg = (void *)&pool;
    record.readings.sensorGroups.funcs.decode = pb_decode_array;
    record.readings.sensorGroups.arg = (void *)&pool;
    return record;
}

fk_data_DataRecord fk_data_record_encoding_new() {
    fk_data_DataRecord record = fk_data_DataRecord_init_default;
    record.metadata.firmware.git.funcs.encode = pb_encode_string;
    record.metadata.firmware.build.funcs.encode = pb_encode_string;
    record.metadata.deviceId.funcs.encode = pb_encode_data;
    record.metadata.generation.funcs.encode = pb_encode_data;
    record.identity.name.funcs.encode = pb_encode_string;
    record.readings.sensorGroups.funcs.encode = pb_encode_array;
    record.modules.funcs.encode = pb_encode_array;
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
    if (reply->status.identity.generation.arg != nullptr) reply->status.identity.generation.funcs.encode = pb_encode_data;

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

    return reply;
}

}
