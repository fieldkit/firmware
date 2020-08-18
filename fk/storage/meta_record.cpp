#include "storage/meta_record.h"
#include "records.h"

namespace fk {

FK_DECLARE_LOGGER("readings");

static void copy_schedule(fk_data_JobSchedule &d, const Schedule &s, Pool &pool) {
    auto intervals = (fk_app_Interval *)pool.malloc(sizeof(fk_app_Interval) * MaximumScheduleIntervals);
    auto intervals_array = pool.malloc_with<pb_array_t>({
        .length = 0,
        .itemSize = sizeof(fk_app_Interval),
        .buffer = intervals,
        .fields = fk_app_Interval_fields,
    });

    for (auto i = 0u; i < MaximumScheduleIntervals; ++i) {
        if (s.intervals[i].interval > 0) {
            intervals_array->length++;
            intervals[i].start = s.intervals[i].start;
            intervals[i].end = s.intervals[i].end;
            intervals[i].interval = s.intervals[i].interval;
        }
        else {
            break;
        }
    }

    if (intervals_array->length > 0) {
        d.intervals.funcs.encode = pb_encode_array;
        d.intervals.arg = (void *)intervals_array;
    }

    d.cron.arg = pb_data_create(s.cron, pool);
    d.interval = s.interval;
    d.duration = s.duration;
    d.jitter = s.jitter;
}

fk_data_DataRecord &MetaRecord::for_decoding(Pool &pool) {
    return record_ = fk_data_record_decoding_new(pool);
}

fk_data_DataRecord const &MetaRecord::record() {
    return record_;
}

void MetaRecord::include_state(GlobalState const *gs, fkb_header_t const *fkb_header, Pool &pool) {
    fk_serial_number_t sn;

    auto device_id_data = pool.malloc_with<pb_data_t>({
        .length = sizeof(sn),
        .buffer = pool.copy(&sn, sizeof(fk_serial_number_t)),
    });

    auto generation_data = pool.malloc_with<pb_data_t>({
        .length = sizeof(gs->general.generation),
        .buffer = gs->general.generation,
    });

    auto hash_size = fkb_header->firmware.hash_size;
    auto hash_hex = bytes_to_hex_string_pool(fkb_header->firmware.hash, hash_size, pool);

    record_ = fk_data_record_encoding_new();
    record_.has_metadata = true;
    record_.metadata.has_firmware = true;
    record_.metadata.firmware.version.arg = (void *)fkb_header->firmware.version;
    record_.metadata.firmware.build.arg = (void *)fkb_header->firmware.name;
    record_.metadata.firmware.hash.arg = (void *)hash_hex;
    record_.metadata.firmware.number.arg = (void *)pool.sprintf("%d", fkb_header->firmware.number);
    record_.metadata.firmware.timestamp = fkb_header->firmware.timestamp;
    record_.metadata.deviceId.arg = (void *)device_id_data;
    record_.metadata.generation.arg = (void *)generation_data;
    record_.has_identity = true;
    record_.identity.name.arg = (void *)gs->general.name;
    record_.has_condition = true;
    record_.condition.flags = fk_data_ConditionFlags_CONDITION_FLAGS_NONE;
    if (gs->general.recording > 0) {
        record_.condition.flags |= fk_data_ConditionFlags_CONDITION_FLAGS_RECORDING;
    }
    record_.condition.recording = gs->general.recording;

    if (gs->lora.configured) {
        auto device_eui_data = pool.malloc_with<pb_data_t>({
            .length = sizeof(gs->lora.device_eui),
            .buffer = gs->lora.device_eui,
        });
        auto app_eui_data = pool.malloc_with<pb_data_t>({
            .length = sizeof(gs->lora.app_eui),
            .buffer = gs->lora.app_eui,
        });
        auto app_key_data = pool.malloc_with<pb_data_t>({
            .length = sizeof(gs->lora.app_key),
            .buffer = gs->lora.app_key,
        });
        auto app_session_key_data = pool.malloc_with<pb_data_t>({
            .length = sizeof(gs->lora.app_session_key),
            .buffer = gs->lora.app_session_key,
        });
        auto network_session_key_data = pool.malloc_with<pb_data_t>({
            .length = sizeof(gs->lora.network_session_key),
            .buffer = gs->lora.network_session_key,
        });
        auto device_address_data = pool.malloc_with<pb_data_t>({
            .length = sizeof(gs->lora.device_address),
            .buffer = gs->lora.device_address,
        });

        record_.has_lora = true;
        record_.lora.deviceEui.arg = (void *)device_eui_data;
        record_.lora.appEui.arg = (void *)app_eui_data;
        record_.lora.appKey.arg = (void *)app_key_data;
        record_.lora.appSessionKey.arg = (void *)app_session_key_data;
        record_.lora.networkSessionKey.arg = (void *)network_session_key_data;
        record_.lora.deviceAddress.arg = (void *)device_address_data;
        record_.lora.uplinkCounter = gs->lora.uplink_counter;
        record_.lora.downlinkCounter = gs->lora.downlink_counter;
    }

    auto networks = pool.malloc<fk_data_NetworkInfo>(WifiMaximumNumberOfNetworks);
    for (auto i = 0u; i < WifiMaximumNumberOfNetworks; ++i) {
        networks[i] = fk_app_NetworkInfo_init_default;
        networks[i].ssid.funcs.encode = pb_encode_string;
        networks[i].ssid.arg = (void *)gs->network.config.wifi_networks[i].ssid;
        networks[i].password.funcs.encode = pb_encode_string;
        networks[i].password.arg = (void *)gs->network.config.wifi_networks[i].password;
    }
    auto networks_array = pool.malloc_with<pb_array_t>({
        .length = WifiMaximumNumberOfNetworks,
        .itemSize = sizeof(fk_data_NetworkInfo),
        .buffer = networks,
        .fields = fk_data_NetworkInfo_fields,
    });

    record_.has_network = true;
    record_.network.networks.arg = (void *)networks_array;

    record_.has_schedule = true;
    record_.schedule.has_readings = true;
    record_.schedule.has_network = true;
    record_.schedule.has_gps = true;
    record_.schedule.has_lora = true;

    copy_schedule(record_.schedule.readings, gs->scheduler.readings, pool);
    copy_schedule(record_.schedule.network, gs->scheduler.network, pool);
    copy_schedule(record_.schedule.gps, gs->scheduler.gps, pool);
    copy_schedule(record_.schedule.lora, gs->scheduler.lora, pool);

    record_.has_transmission = true;
    record_.transmission.has_wifi = true;
    record_.transmission.wifi.enabled = gs->transmission.enabled;
    record_.transmission.wifi.url.arg = (void *)gs->transmission.url;
    record_.transmission.wifi.token.arg = (void *)gs->transmission.token;
}

void MetaRecord::include_modules(GlobalState const *gs, fkb_header_t const *fkb_header, ConstructedModulesCollection &modules, ModuleReadingsCollection &readings, Pool &pool) {
    auto module_infos = pool.malloc<fk_data_ModuleInfo>(modules.size());
    auto readings_iter = readings.begin();

    auto index = 0;
    for (auto &pair : modules) {
        auto &meta = pair.meta;
        auto &module = pair.module;
        if (meta == nullptr || module == nullptr) {
            continue;
        }

        auto sensor_metas = module->get_sensors(pool);

        auto id_data = pool.malloc_with<pb_data_t>({
            .length = sizeof(fk_uuid_t),
            .buffer = &pair.found.header.id,
        });

        auto &m = module_infos[index];
        m = fk_data_ModuleInfo_init_default;
        m.position = pair.found.position;
        m.id.funcs.encode = pb_encode_data;
        m.id.arg = (void *)id_data;
        m.name.funcs.encode = pb_encode_string;
        m.name.arg = (void *)pair.configuration.display_name_key;
        m.has_header = true;
        m.header.manufacturer = meta->manufacturer;
        m.header.kind = meta->kind;
        m.header.version = meta->version;
        m.flags = meta->flags;

        if (readings_iter != readings.end()) {
            auto &mr = *readings_iter;
            if (mr.status_message != nullptr) {
                auto status_message_data = pool.malloc_with<pb_data_t>({
                    .length = mr.status_message->size,
                    .buffer = mr.status_message->buffer,
                });
                m.status.arg = (void *)status_message_data;
            }
            ++readings_iter;
        }
        else {
            logwarn("readings vs modules size mismatch");
        }

        if (sensor_metas != nullptr && sensor_metas->nsensors > 0) {
            auto sensor_infos = pool.malloc<fk_data_SensorInfo>(sensor_metas->nsensors);
            for (size_t i = 0; i < sensor_metas->nsensors; ++i) {
                sensor_infos[i] = fk_data_SensorInfo_init_default;
                sensor_infos[i].name.funcs.encode = pb_encode_string;
                sensor_infos[i].name.arg = (void *)sensor_metas->sensors[i].name;
                sensor_infos[i].unitOfMeasure.funcs.encode = pb_encode_string;
                sensor_infos[i].unitOfMeasure.arg = (void *)sensor_metas->sensors[i].unitOfMeasure;
                sensor_infos[i].flags = sensor_metas->sensors[i].flags;
            }

            auto sensors_array = pool.malloc_with<pb_array_t>({
                .length = sensor_metas->nsensors,
                .itemSize = sizeof(fk_data_SensorInfo),
                .buffer = sensor_infos,
                .fields = fk_data_SensorInfo_fields,
            });

            m.sensors.funcs.encode = pb_encode_array;
            m.sensors.arg = (void *)sensors_array;
        }

        index++;
    }

    auto modules_array = pool.malloc_with<pb_array_t>({
        .length = (size_t)index,
        .itemSize = sizeof(fk_data_ModuleInfo),
        .buffer = module_infos,
        .fields = fk_data_ModuleInfo_fields,
    });

    fk_serial_number_t sn;

    auto device_id_data = pool.malloc_with<pb_data_t>({
        .length = sizeof(sn),
        .buffer = pool.copy(&sn, sizeof(fk_serial_number_t)),
    });

    auto generation_data = pool.malloc_with<pb_data_t>({
        .length = sizeof(gs->general.generation),
        .buffer = gs->general.generation,
    });

    auto hash_size = fkb_header->firmware.hash_size;
    auto hash_hex = bytes_to_hex_string_pool(fkb_header->firmware.hash, hash_size, pool);

    record_ = fk_data_record_encoding_new();
    record_.has_metadata = true;
    record_.metadata.has_firmware = true;
    record_.metadata.firmware.version.arg = (void *)fkb_header->firmware.version;
    record_.metadata.firmware.build.arg = (void *)fkb_header->firmware.name;
    record_.metadata.firmware.hash.arg = (void *)hash_hex;
    record_.metadata.firmware.number.arg = (void *)pool.sprintf("%d", fkb_header->firmware.number);
    record_.metadata.firmware.timestamp = fkb_header->firmware.timestamp;
    record_.metadata.deviceId.arg = (void *)device_id_data;
    record_.metadata.generation.arg = (void *)generation_data;
    record_.has_identity = true;
    record_.identity.name.arg = (void *)gs->general.name;
    record_.modules.arg = (void *)modules_array;
}

}
