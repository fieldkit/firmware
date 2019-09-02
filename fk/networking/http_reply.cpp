#include <loading.h>

#include "networking/http_reply.h"

#include "storage/storage.h"
#include "state.h"
#include "utilities.h"

extern const struct fkb_header_t fkb_header;

namespace fk {

FK_DECLARE_LOGGER("reply");

HttpReply::HttpReply(Pool &pool, GlobalState const *gs) : pool_(&pool), gs_(gs) {
    reply_ = fk_http_reply_encoding();
}

bool HttpReply::include_status() {
    fk_serial_number_t sn;

    auto device_id = pool_->malloc_with<pb_data_t>({
        .length = sizeof(sn),
        .buffer = pool_->copy(&sn, sizeof(sn)),
    });

    reply_.type = fk_app_ReplyType_REPLY_STATUS;
    reply_.status.version = 1;
    reply_.status.uptime = fk_uptime();
    reply_.status.identity.device.arg = (void *)gs_->general.name;
    reply_.status.identity.build.arg = (void *)fkb_header.firmware.name;
    reply_.status.identity.deviceId.arg = device_id;
    reply_.status.power.battery.voltage = 4000;
    reply_.status.power.battery.percentage = 45;

    reply_.status.memory.sramAvailable = fk_free_memory();
    reply_.status.memory.programFlashAvailable = 1024 * 1024 - BootloaderSize - fkb_header.firmware.binary_size;
    reply_.status.memory.extendedMemoryAvailable = 8 * 1024 * 1024;
    reply_.status.memory.dataMemoryInstalled = 512 * 1024 * 1024;
    reply_.status.memory.dataMemoryUsed = 0;
    reply_.status.memory.dataMemoryConsumption = 0;

    if (gs_->modules != nullptr) {
        auto nmodules = gs_->modules->nmodules;
        auto modules = pool_->malloc<fk_app_ModuleCapabilities>(nmodules);
        for (size_t m = 0; m < nmodules; ++m) {
            auto &module = gs_->modules->modules[m];
            auto sensors = pool_->malloc<fk_app_SensorCapabilities>(module.nsensors);
            for (size_t s = 0; s < module.nsensors; ++s) {
                auto &sensor = module.sensors[s];
                sensors[s] = fk_app_SensorCapabilities_init_default;
                sensors[s].number = s;
                sensors[s].module = m;
                sensors[s].name.arg = (void *)sensor.name;
                sensors[s].unitOfMeasure.arg = (void *)sensor.unitOfMeasure;
                sensors[s].flags = sensor.flags;
            }

            auto sensors_array = pool_->malloc_with<pb_array_t>({
               .length = module.nsensors,
               .itemSize = sizeof(fk_app_SensorCapabilities),
               .buffer = sensors,
               .fields = fk_app_SensorCapabilities_fields,
            });

            auto id_data = pool_->malloc_with<pb_data_t>({
                .length = sizeof(fk_uuid_t),
                .buffer = module.id,
            });

            modules[m] = fk_app_ModuleCapabilities_init_default;
            modules[m].position = m;
            modules[m].name.arg = (void *)module.name;
            modules[m].path.arg = (void *)"";
            modules[m].flags = module.flags;
            modules[m].sensors.arg = (void *)sensors_array;
            modules[m].id.arg = (void *)id_data;
        }

        auto modules_array = pool_->malloc_with<pb_array_t>({
            .length = nmodules,
            .itemSize = sizeof(fk_app_ModuleCapabilities),
            .buffer = modules,
            .fields = fk_app_ModuleCapabilities_fields,
        });

        reply_.modules.arg = (void *)modules_array;
    }

    auto &gps = gs_->gps;
    reply_.status.gps.enabled = gps.enabled;
    reply_.status.gps.fix = gps.fix;
    reply_.status.gps.time = gps.time;
    reply_.status.gps.satellites = gps.satellites;
    reply_.status.gps.longitude = gps.longitude;
    reply_.status.gps.latitude = gps.latitude;
    reply_.status.gps.altitude = gps.altitude;

    auto streams = pool_->malloc_with<fk_app_DataStream, 2>({
        {
            .id = 0,
            .time = 0,
            .size = 0,
            .version = 0,
            .block = 0,
            .hash = {
                .funcs = {},
                .arg = nullptr,
            },
            .name = {
                .funcs = {},
                .arg = (void *)"data.fkpb",
            },
            .path = {
                .funcs = {},
                .arg = (void *)"/fk/v1/download/data",
            },
        },
        {
            .id = 1,
            .time = 0,
            .size = 0,
            .version = 0,
            .block = 0,
            .hash = {
                .funcs = {},
                .arg = nullptr,
            },
            .name = {
                .funcs = {},
                .arg = (void *)"meta.fkpb",
            },
            .path = {
                .funcs = {},
                .arg = (void *)"/fk/v1/download/meta",
            },
        },
    });

    streams[Storage::Meta].size = gs_->storage.meta.size;
    streams[Storage::Meta].block = gs_->storage.meta.block;

    streams[Storage::Data].size = gs_->storage.data.size;
    streams[Storage::Data].block = gs_->storage.data.block;

    reply_.status.memory.dataMemoryUsed = streams[Storage::Data].size + streams[Storage::Meta].size;
    reply_.status.memory.dataMemoryConsumption = reply_.status.memory.dataMemoryUsed / reply_.status.memory.dataMemoryInstalled * 100.0f;

    auto streams_array = pool_->malloc_with<pb_array_t>({
        .length = (size_t)2,
        .itemSize = sizeof(fk_app_DataStream),
        .buffer = streams,
        .fields = fk_app_DataStream_fields,
    });

    reply_.streams.funcs.encode = pb_encode_array;
    reply_.streams.arg = (void *)streams_array;

    if (fkb_header.firmware.hash_size > 0) {
        auto firmware_hash_string = bytes_to_hex_string_pool(fkb_header.firmware.hash, fkb_header.firmware.hash_size, *pool_);
        reply_.status.identity.firmware.arg = (void *)firmware_hash_string;
    }

    return true;
}

bool HttpReply::include_readings() {
    if (gs_->modules == nullptr) {
        loginfo("no live readings");
        return true;
    }

    auto nmodules = gs_->modules->nmodules;
    auto lmr = pool_->malloc<fk_app_LiveModuleReadings>(nmodules);

    for (size_t m = 0; m < nmodules; ++m) {
        auto &module = gs_->modules->modules[m];
        auto nreadings = module.nsensors;
        auto readings = pool_->malloc<fk_app_LiveSensorReading>(nreadings);

        for (size_t s = 0; s < nreadings; ++s) {
            auto &sensor = module.sensors[s];
            readings[s] = fk_app_LiveSensorReading_init_default;
            readings[s].sensor = fk_app_SensorCapabilities_init_default;
            readings[s].sensor.number = s;
            readings[s].sensor.name.arg = (void *)sensor.name;
            readings[s].sensor.unitOfMeasure.arg = (void *)sensor.unitOfMeasure;
            readings[s].sensor.flags = sensor.flags;
            if (sensor.has_live_vaue) {
                readings[s].value = sensor.live_value;
            }
        }

        auto id_data = pool_->malloc_with<pb_data_t>({
            .length = sizeof(fk_uuid_t),
            .buffer = module.id,
        });

        auto readings_array = pool_->malloc_with<pb_array_t>({
            .length = nreadings,
            .itemSize = sizeof(fk_app_LiveSensorReading),
            .buffer = readings,
            .fields = fk_app_LiveSensorReading_fields,
        });

        lmr[m] = fk_app_LiveModuleReadings_init_default;
        lmr[m].module = fk_app_ModuleCapabilities_init_default;
        lmr[m].module.position = m;
        lmr[m].module.id.arg = (void *)id_data;
        lmr[m].module.name.arg = (void *)module.name;
        lmr[m].module.flags = module.flags;
        lmr[m].readings.arg = (void *)readings_array;
    }

    auto lmr_array = pool_->malloc_with<pb_array_t>({
        .length = (size_t)nmodules,
        .itemSize = sizeof(fk_app_LiveModuleReadings),
        .buffer = lmr,
        .fields = fk_app_LiveModuleReadings_fields,
    });

    auto live_readings = pool_->malloc<fk_app_LiveReadings>();
    live_readings[0] = fk_app_LiveReadings_init_default;
    live_readings[0].time = 0;
    live_readings[0].modules.arg = (void *)lmr_array;

    auto live_readings_array = pool_->malloc_with<pb_array_t>({
        .length = (size_t)1,
        .itemSize = sizeof(fk_app_LiveReadings),
        .buffer = live_readings,
        .fields = fk_app_LiveReadings_fields,
    });

    reply_.type = fk_app_ReplyType_REPLY_READINGS;
    reply_.liveReadings.arg = (void *)live_readings_array;

    return true;
}

}