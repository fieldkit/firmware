#include "storage/meta_record.h"
#include "records.h"

namespace fk {

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

    auto networks = pool.malloc<fk_data_NetworkInfo>(MaximumNumberOfWifiNetworks);
    for (auto i = 0u; i < MaximumNumberOfWifiNetworks; ++i) {
        networks[i] = fk_app_NetworkInfo_init_default;
        networks[i].ssid.funcs.encode = pb_encode_string;
        networks[i].ssid.arg = (void *)gs->network.config.wifi_networks[i].ssid;
        networks[i].password.funcs.encode = pb_encode_string;
        networks[i].password.arg = (void *)gs->network.config.wifi_networks[i].password;
    }
    auto networks_array = pool.malloc_with<pb_array_t>({
        .length = MaximumNumberOfWifiNetworks,
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

    record_.schedule.readings.cron.arg = pb_data_create(gs->scheduler.readings.cron, pool);
    record_.schedule.network.cron.arg = pb_data_create(gs->scheduler.network.cron, pool);
    record_.schedule.gps.cron.arg = pb_data_create(gs->scheduler.gps.cron, pool);
    record_.schedule.lora.cron.arg = pb_data_create(gs->scheduler.lora.cron, pool);

    record_.schedule.readings.interval = gs->scheduler.readings.interval;
    record_.schedule.network.interval = gs->scheduler.network.interval;
    record_.schedule.gps.interval = gs->scheduler.gps.interval;
    record_.schedule.lora.interval = gs->scheduler.lora.interval;

    record_.schedule.readings.duration = gs->scheduler.readings.duration;
    record_.schedule.network.duration = gs->scheduler.network.duration;
    record_.schedule.gps.duration = gs->scheduler.gps.duration;
    record_.schedule.lora.duration = gs->scheduler.lora.duration;
}

void MetaRecord::include_modules(GlobalState const *gs, fkb_header_t const *fkb_header, ConstructedModulesCollection &modules, Pool &pool) {
    auto module_infos = pool.malloc<fk_data_ModuleInfo>(modules.size());

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
        m.name.arg = (void *)meta->name;
        m.has_header = true;
        m.header.manufacturer = meta->manufacturer;
        m.header.kind = meta->kind;
        m.header.version = meta->version;

        if (sensor_metas != nullptr && sensor_metas->nsensors > 0) {
            auto sensor_infos = pool.malloc<fk_data_SensorInfo>(sensor_metas->nsensors);
            for (size_t i = 0; i < sensor_metas->nsensors; ++i) {
                sensor_infos[i] = fk_data_SensorInfo_init_default;
                sensor_infos[i].name.funcs.encode = pb_encode_string;
                sensor_infos[i].name.arg = (void *)sensor_metas->sensors[i].name;
                sensor_infos[i].unitOfMeasure.funcs.encode = pb_encode_string;
                sensor_infos[i].unitOfMeasure.arg = (void *)sensor_metas->sensors[i].unitOfMeasure;
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
