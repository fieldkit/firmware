#include <loading.h>

#include "storage/meta_ops.h"
#include "utilities.h"
#include "records.h"
#include "state.h"

extern const struct fkb_header_t fkb_header;

namespace fk {

FK_DECLARE_LOGGER("meta");

MetaOps::MetaOps(Storage &storage) : storage_(storage) {
}

tl::expected<uint32_t, Error> MetaOps::write_state(GlobalState const *gs, Pool &pool) {
    fk_serial_number_t sn;
    pb_data_t device_id = {
        .length = sizeof(sn),
        .buffer = &sn,
    };

    pb_data_t generation = {
        .length = sizeof(gs->general.generation),
        .buffer = gs->general.generation,
    };

    auto hash_size = fkb_header.firmware.hash_size;
    auto hash_hex = bytes_to_hex_string_pool(fkb_header.firmware.hash, hash_size, pool);

    auto record = fk_data_record_encoding_new();
    record.metadata.firmware.version.arg = (void *)fkb_header.firmware.version;
    record.metadata.firmware.build.arg = (void *)fkb_header.firmware.name;
    record.metadata.firmware.hash.arg = (void *)hash_hex;
    record.metadata.firmware.number.arg = (void *)pool.sprintf("%d", fkb_header.firmware.number);
    record.metadata.firmware.timestamp = fkb_header.firmware.timestamp;
    record.metadata.deviceId.arg = (void *)&device_id;
    record.metadata.generation.arg = (void *)&generation;
    record.identity.name.arg = (void *)gs->general.name;
    record.condition.flags = fk_data_ConditionFlags_CONDITION_FLAGS_NONE;
    if (gs->general.recording > 0) {
        record.condition.flags |= fk_data_ConditionFlags_CONDITION_FLAGS_RECORDING;
    }
    record.condition.recording = gs->general.recording;

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

        record.lora.deviceEui.arg = (void *)device_eui_data;
        record.lora.appEui.arg = (void *)app_eui_data;
        record.lora.appKey.arg = (void *)app_key_data;
        record.lora.appSessionKey.arg = (void *)app_session_key_data;
        record.lora.networkSessionKey.arg = (void *)network_session_key_data;
        record.lora.deviceAddress.arg = (void *)device_address_data;
        record.lora.uplinkCounter= gs->lora.uplink_counter;
        record.lora.downlinkCounter= gs->lora.downlink_counter;
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

    record.network.networks.arg = (void *)networks_array;

    record.schedule.readings.cron.arg = pb_data_create(gs->scheduler.readings.cron, pool);
    record.schedule.network.cron.arg = pb_data_create(gs->scheduler.network.cron, pool);
    record.schedule.gps.cron.arg = pb_data_create(gs->scheduler.gps.cron, pool);
    record.schedule.lora.cron.arg = pb_data_create(gs->scheduler.lora.cron, pool);

    record.schedule.readings.interval = gs->scheduler.readings.interval;
    record.schedule.network.interval = gs->scheduler.network.interval;
    record.schedule.gps.interval = gs->scheduler.gps.interval;
    record.schedule.lora.interval = gs->scheduler.lora.interval;

    auto meta = storage_.file(Storage::Meta);
    if (!meta.seek_end()) {
        FK_ASSERT(meta.create());
    }

    auto srl = SignedRecordLog{ meta };
    auto meta_record = srl.append_immutable(SignedRecordKind::State, &record, fk_data_DataRecord_fields, pool);
    if (!meta_record) {
        return tl::unexpected<Error>(meta_record.error());
    }

    char gen_string[GenerationLength * 2 + 1];
    bytes_to_hex_string(gen_string, sizeof(gen_string), gs->general.generation, sizeof(gs->general.generation));
    loginfo("(saved) name: '%s'", gs->general.name);
    loginfo("(saved) gen: %s", gen_string);

    if (gs->general.recording) {
        loginfo("(saved) recording");
    }

    return (*meta_record).record;
}

tl::expected<uint32_t, Error> MetaOps::write_modules(GlobalState const *gs, ConstructedModulesCollection &modules, Pool &pool) {
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
        m.position = index;
        m.id.funcs.encode = pb_encode_data;
        m.id.arg = (void *)id_data;
        m.name.funcs.encode = pb_encode_string;
        m.name.arg = (void *)meta->name;
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
        .buffer = &sn,
    });

    auto generation_data = pool.malloc_with<pb_data_t>({
        .length = sizeof(gs->general.generation),
        .buffer = gs->general.generation,
    });

    auto hash_size = fkb_header.firmware.hash_size;
    auto hash_hex = bytes_to_hex_string_pool(fkb_header.firmware.hash, hash_size, pool);

    auto record = fk_data_record_encoding_new();
    record.metadata.firmware.version.arg = (void *)fkb_header.firmware.version;
    record.metadata.firmware.build.arg = (void *)fkb_header.firmware.name;
    record.metadata.firmware.hash.arg = (void *)hash_hex;
    record.metadata.firmware.number.arg = (void *)pool.sprintf("%d", fkb_header.firmware.number);
    record.metadata.firmware.timestamp = fkb_header.firmware.timestamp;
    record.metadata.deviceId.arg = (void *)device_id_data;
    record.metadata.generation.arg = (void *)generation_data;
    record.identity.name.arg = (void *)gs->general.name;
    record.modules.arg = (void *)modules_array;

    auto meta = storage_.file(Storage::Meta);
    if (!meta.seek_end()) {
        FK_ASSERT(meta.create());
    }
    auto srl = SignedRecordLog { meta };

    auto meta_record = srl.append_immutable(SignedRecordKind::Modules, &record, fk_data_DataRecord_fields, pool);

    return (*meta_record).record;
}

}
