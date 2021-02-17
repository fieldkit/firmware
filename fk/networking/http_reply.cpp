#include <loading.h>

#include "hal/network.h"

#include "networking/http_reply.h"

#include "storage/storage.h"
#include "state.h"
#include "utilities.h"
#include "clock.h"

extern const struct fkb_header_t fkb_header;

namespace fk {

FK_DECLARE_LOGGER("reply");

HttpReply::HttpReply(Pool &pool, GlobalState const *gs) : pool_(&pool), gs_(gs) {
    reply_ = fk_http_reply_encoding();
}

bool HttpReply::include_success(uint32_t clock, uint32_t uptime) {
    reply_.type = fk_app_ReplyType_REPLY_SUCCESS;
    reply_.status.version = 1;
    reply_.status.uptime = uptime;
    reply_.status.time = clock;

    return true;
}

static void copy_schedule(fk_app_Schedule &d, Schedule const &s, Pool *pool) {
    auto intervals = (fk_app_Interval *)pool->malloc(sizeof(fk_app_Interval) * MaximumScheduleIntervals);
    auto intervals_array = pool->malloc_with<pb_array_t>({
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

    d.interval = s.interval;
    d.duration = s.duration;
}

bool HttpReply::include_status(uint32_t clock, uint32_t uptime, bool logs, fkb_header_t const *fkb) {
    fk_serial_number_t sn;

    auto device_id_data = pool_->malloc_with<pb_data_t>({
        .length = sizeof(sn),
        .buffer = pool_->copy(&sn, sizeof(sn)),
    });

    auto generation_data = pool_->malloc_with<pb_data_t>({
        .length = sizeof(gs_->general.generation),
        .buffer = &gs_->general.generation,
    });

    reply_.type = fk_app_ReplyType_REPLY_STATUS;
    reply_.has_status = true;
    reply_.status.time = clock;
    reply_.status.version = 1;
    reply_.status.uptime = uptime;
    reply_.status.has_identity = true;
    reply_.status.identity.name.arg = (void *)gs_->general.name;
    reply_.status.identity.device.arg = (void *)gs_->general.name;
    reply_.status.identity.deviceId.arg = device_id_data;
    reply_.status.identity.generationId.arg = generation_data;
    if (logs) {
        reply_.status.logs.arg = 0; // This is used to store the calculated size.
        reply_.status.logs.funcs.encode = pb_encode_logs;
    }

    if (fkb->firmware.hash_size > 0) {
        auto firmware_hash_string = bytes_to_hex_string_pool(fkb->firmware.hash, fkb->firmware.hash_size, *pool_);
        reply_.status.identity.firmware.arg = (void *)firmware_hash_string;
    }

    reply_.status.has_firmware = true;
    reply_.status.firmware.version.arg = (void *)fkb->firmware.version;
    reply_.status.firmware.build.arg = (void *)fkb->firmware.name;
    reply_.status.firmware.number.arg = (void *)pool_->sprintf("%d", fkb->firmware.number);
    reply_.status.firmware.timestamp = fkb->firmware.timestamp;
    reply_.status.firmware.hash.arg = (void *)bytes_to_hex_string_pool(fkb->firmware.hash, fkb->firmware.hash_size, *pool_);

    loginfo("(reply) firmware: number=%" PRIu32 " build=%s", fkb->firmware.number, fkb->firmware.name);

    reply_.status.has_power = true;
    reply_.status.power.has_battery = true;
    reply_.status.power.battery.voltage = gs_->power.battery.bus_voltage * 1000.0f;
    reply_.status.power.battery.percentage = gs_->power.charge;
    reply_.status.power.has_solar = true;
    reply_.status.power.solar.voltage = gs_->power.solar.bus_voltage * 1000.0f;

    reply_.status.has_recording = true;
    reply_.status.recording.enabled = gs_->general.recording > 0;
    reply_.status.recording.startedTime = gs_->general.recording;

    reply_.status.has_memory = true;
    reply_.status.memory.sramAvailable = fk_free_memory();
    reply_.status.memory.programFlashAvailable = 1024 * 1024 - BootloaderSize - fkb_header.firmware.binary_size;
    reply_.status.memory.extendedMemoryAvailable = 8 * 1024 * 1024;
    reply_.status.memory.dataMemoryInstalled = 512 * 1024 * 1024;
    reply_.status.memory.dataMemoryUsed = 0;
    reply_.status.memory.dataMemoryConsumption = 0;

    if (gs_->modules != nullptr && gs_->modules->nmodules > 0) {
        auto nmodules = gs_->modules->nmodules;
        FK_ASSERT(nmodules < 10); // NOTE: Just a sane number. Chasing a crash.
        auto modules = pool_->malloc<fk_app_ModuleCapabilities>(nmodules);
        for (size_t m = 0; m < nmodules; ++m) {
            auto &module = gs_->modules->modules[m];

            auto sensors_array = pool_->malloc_with<pb_array_t>({
                .length = module.nsensors,
                .itemSize = sizeof(fk_app_SensorCapabilities),
                .buffer = nullptr,
                .fields = fk_app_SensorCapabilities_fields,
            });

            if (module.nsensors > 0) {
                auto sensors = pool_->malloc<fk_app_SensorCapabilities>(module.nsensors);
                for (size_t s = 0; s < module.nsensors; ++s) {
                    auto &sensor = module.sensors[s];
                    sensors[s] = fk_app_SensorCapabilities_init_default;
                    sensors[s].number = s;
                    sensors[s].name.arg = (void *)sensor.name;
                    sensors[s].unitOfMeasure.arg = (void *)sensor.unit_of_measure;
                    sensors[s].flags = sensor.flags;
                }

                sensors_array->buffer = sensors;
            }

            auto id_data = pool_->malloc_with<pb_data_t>({
                .length = sizeof(fk_uuid_t),
                .buffer = module.id,
            });

            modules[m] = fk_app_ModuleCapabilities_init_default;
            modules[m].position = module.position.integer();
            modules[m].name.arg = (void *)module.display_name_key;
            modules[m].path.arg = (void *)pool_->sprintf("/fk/v1/modules/%d", module.position.integer());
            modules[m].flags = module.flags;
            modules[m].id.arg = (void *)id_data;
            modules[m].has_header = true;
            modules[m].header.manufacturer = module.manufacturer;
            modules[m].header.kind = module.kind;
            modules[m].header.version = module.version;
            if (sensors_array->buffer != nullptr) {
                modules[m].sensors.arg = (void *)sensors_array;
            }

            if (module.configuration_message != nullptr) {
                auto configuration_message_data = pool_->malloc_with<pb_data_t>({
                    .length = module.configuration_message->size,
                    .buffer = module.configuration_message->buffer,
                });
                modules[m].configuration.arg = (void *)configuration_message_data;
            }
        }

        auto modules_array = pool_->malloc_with<pb_array_t>({
            .length = nmodules,
            .itemSize = sizeof(fk_app_ModuleCapabilities),
            .buffer = modules,
            .fields = fk_app_ModuleCapabilities_fields,
        });

        reply_.modules.arg = (void *)modules_array;
    }
    else {
        logwarn("no modules");
    }

    auto &gps = gs_->gps;
    reply_.status.has_gps = true;
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

    auto device_eui_data = pool_->malloc_with<pb_data_t>({
        .length = sizeof(gs_->lora.device_eui),
        .buffer = gs_->lora.device_eui,
    });
    auto app_eui_data = pool_->malloc_with<pb_data_t>({
        .length = sizeof(gs_->lora.app_eui),
        .buffer = gs_->lora.app_eui,
    });
    auto app_key_data = pool_->malloc_with<pb_data_t>({
        .length = sizeof(gs_->lora.app_key),
        .buffer = gs_->lora.app_key,
    });

    reply_.has_loraSettings = true;
    reply_.loraSettings.deviceEui.funcs.encode = pb_encode_data;
    reply_.loraSettings.deviceEui.arg = (void *)device_eui_data;
    reply_.loraSettings.appEui.funcs.encode = pb_encode_data;
    reply_.loraSettings.appEui.arg = (void *)app_eui_data;
    reply_.loraSettings.appKey.funcs.encode = pb_encode_data;
    reply_.loraSettings.appKey.arg = (void *)app_key_data;

    auto nnetworks = 0u;
    auto networks = pool_->malloc<fk_app_NetworkInfo>(WifiMaximumNumberOfNetworks);
    for (auto i = 0u; i < WifiMaximumNumberOfNetworks; ++i) {
        if (strlen(gs_->network.config.wifi_networks[i].ssid) > 0) {
            networks[nnetworks] = fk_app_NetworkInfo_init_default;
            networks[nnetworks].ssid.funcs.encode = pb_encode_string;
            networks[nnetworks].ssid.arg = (void *)gs_->network.config.wifi_networks[i].ssid;
            networks[nnetworks].password.funcs.encode = pb_encode_string;
            networks[nnetworks].password.arg = (void *)gs_->network.config.wifi_networks[i].password;
            loginfo("(reply) [%d] network: %s", nnetworks, gs_->network.config.wifi_networks[i].ssid);
            nnetworks++;
        }
    }

    auto networks_array = pool_->malloc_with<pb_array_t>({
        .length = nnetworks,
        .itemSize = sizeof(fk_app_NetworkInfo),
        .buffer = networks,
        .fields = fk_app_NetworkInfo_fields,
    });

    reply_.has_networkSettings = true;
    reply_.networkSettings.has_connected = true;
    reply_.networkSettings.connected.create = get_network()->get_created_ap();
    reply_.networkSettings.connected.ssid.arg = (void *)get_network()->get_ssid();
    reply_.networkSettings.networks.funcs.encode = pb_encode_array;
    reply_.networkSettings.networks.arg = (void *)networks_array;
    uint8_t mac_address[6];
    if (get_network()->get_mac_address(mac_address)) {
        auto mac_address_string = bytes_to_hex_string_pool(mac_address, sizeof(mac_address), *pool_);
        reply_.networkSettings.macAddress.arg = (void *)mac_address_string;
    }

    reply_.has_schedules = true;
    reply_.schedules.has_readings = true;
    reply_.schedules.has_network = true;
    reply_.schedules.has_gps = true;
    reply_.schedules.has_lora = true;

    copy_schedule(reply_.schedules.readings, gs_->scheduler.readings, pool_);
    copy_schedule(reply_.schedules.network, gs_->scheduler.network, pool_);
    copy_schedule(reply_.schedules.gps, gs_->scheduler.gps, pool_);
    copy_schedule(reply_.schedules.lora, gs_->scheduler.lora, pool_);

    if (strlen(gs_->transmission.url) > 0 || strlen(gs_->transmission.token) > 0) {
        reply_.has_transmission = true;
        reply_.transmission.has_wifi = true;
        reply_.transmission.wifi.url.arg = (void *)gs_->transmission.url;
        reply_.transmission.wifi.token.arg = (void *)gs_->transmission.token;
    }
    reply_.transmission.wifi.enabled = gs_->transmission.enabled;

    return true;
}

bool HttpReply::include_readings() {
    if (gs_->modules == nullptr) {
        loginfo("no live readings");
        return true;
    }

    auto nmodules = gs_->modules->nmodules;
    if (nmodules == 0) {
        loginfo("no modules");
        return true;
    }

    auto lmr = pool_->malloc<fk_app_LiveModuleReadings>(nmodules);

    for (auto m = 0u; m < nmodules; ++m) {
        auto &module = gs_->modules->modules[m];

        auto id_data = pool_->malloc_with<pb_data_t>({
            .length = sizeof(fk_uuid_t),
            .buffer = module.id,
        });

        lmr[m] = fk_app_LiveModuleReadings_init_default;
        lmr[m].has_module = true;
        lmr[m].module = fk_app_ModuleCapabilities_init_default;
        lmr[m].module.position = module.position.integer();
        lmr[m].module.id.arg = (void *)id_data;
        lmr[m].module.name.arg = (void *)module.display_name_key;
        lmr[m].module.path.arg = (void *)pool_->sprintf("/fk/v1/modules/%d", module.position.integer());
        lmr[m].module.flags = module.flags;
        lmr[m].module.has_header = true;
        lmr[m].module.header.manufacturer = module.manufacturer;
        lmr[m].module.header.kind = module.kind;
        lmr[m].module.header.version = module.version;

        auto nreadings = module.nsensors;
        if (nreadings > 0) {
            auto readings = pool_->malloc<fk_app_LiveSensorReading>(nreadings);

            for (auto s = 0u; s < nreadings; ++s) {
                auto &sensor = module.sensors[s];
                readings[s] = fk_app_LiveSensorReading_init_default;
                readings[s].has_sensor = true;
                readings[s].sensor = fk_app_SensorCapabilities_init_default;
                readings[s].sensor.number = s;
                readings[s].sensor.name.arg = (void *)sensor.name;
                readings[s].sensor.unitOfMeasure.arg = (void *)sensor.unit_of_measure;
                readings[s].sensor.flags = sensor.flags;
                if (sensor.has_live_vaue) {
                    readings[s].value = sensor.live_value;
                }
            }

            auto readings_array = pool_->malloc_with<pb_array_t>({
                .length = nreadings,
                .itemSize = sizeof(fk_app_LiveSensorReading),
                .buffer = readings,
                .fields = fk_app_LiveSensorReading_fields,
            });

            lmr[m].readings.arg = (void *)readings_array;
        }
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
    reply_.has_status = true;
    reply_.status.time = get_clock_now();
    reply_.liveReadings.arg = (void *)live_readings_array;

    return true;
}

bool HttpReply::include_scan(NetworkScan scan) {
    auto nearby_networks_array = pool_->malloc_with<pb_array_t>({
        .length = scan.length(),
        .itemSize = sizeof(fk_app_NearbyNetwork),
        .buffer = nullptr,
        .fields = fk_app_NearbyNetwork_fields,
    });

    if (scan.length() > 0) {
        auto nearby = (fk_app_NearbyNetwork *)pool_->malloc(sizeof(fk_app_NearbyNetwork) * scan.length());
        for (auto i = 0u; i < scan.length(); ++i) {
            loginfo("[%u]: network: %s", i, scan.network(i));
            nearby[i].ssid.arg = (void *)scan.network(i);
        }
        nearby_networks_array->buffer = nearby;
    }

    reply_.has_nearbyNetworks = true;
    reply_.nearbyNetworks.networks.arg = (void *)nearby_networks_array;

    reply_.type = fk_app_ReplyType_REPLY_NETWORKS;

    return true;
}

bool HttpReply::include_listing(const char *path, fk_app_DirectoryEntry *entries, size_t number_entries, size_t total_entries) {
    reply_ = fk_app_HttpReply_init_default;
    reply_.type = fk_app_ReplyType_REPLY_FILES;

    if (number_entries == 0) {
        return true;
    }

    auto entries_array = pool_->malloc_with<pb_array_t>({
        .length = number_entries,
        .itemSize = sizeof(fk_app_DirectoryEntry),
        .buffer = entries,
        .fields = fk_app_DirectoryEntry_fields,
    });

    loginfo("returning listing %zu", number_entries);

    reply_.has_listing = true;
    reply_.listing.totalEntries = total_entries;
    reply_.listing.path.arg = (void *)path;
    reply_.listing.path.funcs.encode = pb_encode_string;
    reply_.listing.entries.arg = (void *)entries_array;
    reply_.listing.entries.funcs.encode = pb_encode_array;

    return true;
}

}
