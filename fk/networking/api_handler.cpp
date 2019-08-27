#include <tiny_printf.h>
#include <loading.h>

#include "networking/api_handler.h"
#include "storage/storage.h"
#include "protobuf.h"
#include "utilities.h"
#include "storage/signed_log.h"
#include "records.h"
#include "readings_worker.h"
#include "device_name.h"

extern const struct fkb_header_t fkb_header;

namespace fk {

FK_DECLARE_LOGGER("api");

static bool send_status(HttpRequest &req, fk_app_HttpQuery *query, Pool &pool);

static bool send_readings(HttpRequest &req, fk_app_HttpQuery *query, Pool &pool);

static bool configure(HttpRequest &req, fk_app_HttpQuery *query, Pool &pool);

bool ApiHandler::handle(HttpRequest &req, Pool &pool) {
    auto reader = req.reader();
    if (req.content_type() == WellKnownContentType::TextPlain) {
        reader = new (pool) Base64Reader(req.reader());
        req.connection()->hex_encoding(true);
    }

    auto query = fk_http_query_prepare_decoding(pool.malloc<fk_app_HttpQuery>(), &pool);
    auto stream = pb_istream_from_readable(reader);
    if (!pb_decode_delimited(&stream, fk_app_HttpQuery_fields, query)) {
        req.connection()->error("error parsing query");
        return true;
    }

    switch (query->type) {
    case fk_app_QueryType_QUERY_STATUS: {
        loginfo("handling %s", "QUERY_STATUS");
        return send_status(req, query, pool);
    }
    case fk_app_QueryType_QUERY_CONFIGURE: {
        loginfo("handling %s", "QUERY_CONFIGURE");
        return configure(req, query, pool);
    }
    case fk_app_QueryType_QUERY_TAKE_READINGS: {
        loginfo("handling %s", "QUERY_TAKE_READINGS");
        // TODO: MALLOC
        if (!get_ipc()->launch_worker(new ReadingsWorker())) {
            return false;
        }
        return send_readings(req, query, pool);
    }
    case fk_app_QueryType_QUERY_GET_READINGS: {
        loginfo("handling %s", "QUERY_GET_READINGS");
        return send_readings(req, query, pool);
    }
    default: {
        break;
    }
    }

    req.connection()->error("unknown query type");
    return true;
}

static bool configure(HttpRequest &req, fk_app_HttpQuery *query, Pool &pool) {
    if (query->identity.name.arg != nullptr) {
        auto name = (char *)query->identity.name.arg;

        StatisticsMemory memory{ MemoryFactory::get_data_memory() };
        Storage storage{ &memory };
        if (!storage.begin()) {
            return false;
        }

        auto meta = storage.file(Storage::Meta);
        if (!meta.seek_end()) {
            FK_ASSERT(meta.create());
        }

        fk_serial_number_t sn;
        fk_serial_number_get(&sn);

        pb_data_t device_id = {
            .length = sizeof(sn),
            .buffer = &sn,
        };

        auto hash_size = fkb_header.firmware.hash_size;
        auto hash_hex = bytes_to_hex_string_pool(fkb_header.firmware.hash, hash_size, pool);

        auto record = fk_data_record_encoding_new();
        record.metadata.firmware.git.arg = (void *)hash_hex;
        record.metadata.firmware.build.arg = (void *)fkb_header.firmware.name;
        record.metadata.deviceId.arg = (void *)&device_id;
        record.identity.name.arg = name;

        auto srl = SignedRecordLog{ meta };
        if (!srl.append_immutable(SignedRecordKind::State, &record, fk_data_DataRecord_fields, pool)) {
            return false;
        }
    }

    return send_status(req, query, pool);
}

static bool send_status(HttpRequest &req, fk_app_HttpQuery *query, Pool &pool) {
    constexpr static uint32_t BootloaderSize = 0x4000;

    auto lock = storage_mutex.acquire(UINT32_MAX);
    auto memory_bus = get_board()->spi_flash();
    auto gs = get_global_state_ro();

    fk_serial_number_t sn;
    fk_serial_number_get(&sn);

    pb_data_t device_id = {
        .length = sizeof(sn),
        .buffer = &sn,
    };

    auto name = fk_device_name_generate(pool);

    auto reply = fk_http_reply_encoding();
    reply.type = fk_app_ReplyType_REPLY_STATUS;
    reply.status.version = 1;
    reply.status.uptime = fk_uptime();
    reply.status.identity.device.arg = (void *)name;
    reply.status.identity.build.arg = (void *)fkb_header.firmware.name;
    reply.status.identity.deviceId.arg = &device_id;
    reply.status.power.battery.voltage = 4000;
    reply.status.power.battery.percentage = 45;

    reply.status.memory.sramAvailable = fk_free_memory();
    reply.status.memory.programFlashAvailable = 1024 * 1024 - BootloaderSize - fkb_header.firmware.binary_size;
    reply.status.memory.extendedMemoryAvailable = 8 * 1024 * 1024;
    reply.status.memory.dataMemoryInstalled = 512 * 1024 * 1024;
    reply.status.memory.dataMemoryUsed = 0;
    reply.status.memory.dataMemoryConsumption = 0;

    auto &gps = gs.get()->gps;
    reply.status.gps.enabled = gps.enabled;
    reply.status.gps.fix = gps.fix;
    reply.status.gps.time = gps.time;
    reply.status.gps.satellites = gps.satellites;
    reply.status.gps.longitude = gps.longitude;
    reply.status.gps.latitude = gps.latitude;
    reply.status.gps.altitude = gps.altitude;

    fk_app_DataStream streams[] = {
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
                .funcs = {
                    .encode = pb_encode_string,
                },
                .arg = (void *)"data.fkpb",
            },
            .path = {
                .funcs = {
                    .encode = pb_encode_string,
                },
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
                .funcs = {
                    .encode = pb_encode_string,
                },
                .arg = (void *)"meta.fkpb",
            },
            .path = {
                .funcs = {
                    .encode = pb_encode_string,
                },
                .arg = (void *)"/fk/v1/download/meta",
            },
        },
    };

    StatisticsMemory memory{ MemoryFactory::get_data_memory() };
    Storage storage{ &memory };
    if (storage.begin()) {
        auto meta = storage.file(Storage::Meta);
        auto srl = SignedRecordLog{ meta };
        if (srl.seek_record(SignedRecordKind::State)) {
            auto record = fk_data_record_decoding_new(pool);
            record.identity.name.arg = (void *)&pool;
            if (srl.decode(&record, fk_data_DataRecord_fields, pool)) {
                reply.status.identity.device.arg = record.identity.name.arg;
            }
        }
    }

    for (auto file_number : { Storage::Data, Storage::Meta }) {
        auto file = storage.file(file_number);
        if (file.seek_end()) {
            auto &stream = streams[file_number];
            stream.size = file.size();
            stream.block = file.record();
            reply.status.memory.dataMemoryUsed += stream.size;
        }
    }

    reply.status.memory.dataMemoryConsumption = reply.status.memory.dataMemoryUsed / reply.status.memory.dataMemoryInstalled * 100.0f;

    pb_array_t streams_array = {
        .length = (size_t)2,
        .itemSize = sizeof(fk_app_DataStream),
        .buffer = &streams,
        .fields = fk_app_DataStream_fields,
    };

    reply.streams.funcs.encode = pb_encode_array;
    reply.streams.arg = (void *)&streams_array;

    if (fkb_header.firmware.hash_size > 0) {
        char firmware_hash_string[128];
        bytes_to_hex_string(firmware_hash_string, sizeof(firmware_hash_string),
                            fkb_header.firmware.hash, fkb_header.firmware.hash_size);
        reply.status.identity.firmware.arg = (void *)firmware_hash_string;
    }

    req.connection()->write(&reply);
    req.connection()->close();

    memory.log_statistics();

    return true;
}

bool send_readings(HttpRequest &req, fk_app_HttpQuery *query, Pool &pool) {
    auto gs = get_global_state_ro();

    if (gs.get()->modules == nullptr) {
        loginfo("no live readings");
        return false;
    }

    auto nmodules = gs.get()->modules->nmodules;
    auto lmr = pool.malloc<fk_app_LiveModuleReadings>(nmodules);

    for (size_t i = 0; i < nmodules; ++i) {
        auto &module = gs.get()->modules->modules[i];
        auto nreadings = module.nsensors;
        auto readings = pool.malloc<fk_app_LiveSensorReading>(nreadings);

        for (size_t j = 0; j < nreadings; ++j) {
            auto &sensor = module.sensors[j];
            readings[j] = fk_app_LiveSensorReading_init_default;
            readings[j].sensor = fk_app_SensorCapabilities_init_default;
            readings[j].sensor.name.arg = (void *)sensor.name;
            readings[j].sensor.unitOfMeasure.arg = (void *)sensor.unitOfMeasure;
            if (sensor.has_live_vaue) {
                readings[j].value = sensor.live_value;
            }
        }

        auto readings_array = pool.malloc_with<pb_array_t>({
            .length = nreadings,
            .itemSize = sizeof(fk_app_LiveSensorReading),
            .buffer = readings,
            .fields = fk_app_LiveSensorReading_fields,
        });

        lmr[i] = fk_app_LiveModuleReadings_init_default;
        lmr[i].module = fk_app_ModuleCapabilities_init_default;
        lmr[i].readings.arg = (void *)readings_array;
    }

    pb_array_t lmr_array = {
        .length = (size_t)nmodules,
        .itemSize = sizeof(fk_app_LiveModuleReadings),
        .buffer = lmr,
        .fields = fk_app_LiveModuleReadings_fields,
    };

    auto live_readings = pool.malloc<fk_app_LiveReadings>();
    live_readings[0] = fk_app_LiveReadings_init_default;
    live_readings[0].time = 0;
    live_readings[0].modules.arg = (void *)&lmr_array;

    pb_array_t live_readings_array = {
        .length = (size_t)1,
        .itemSize = sizeof(fk_app_LiveReadings),
        .buffer = live_readings,
        .fields = fk_app_LiveReadings_fields,
    };

    auto reply = fk_http_reply_encoding();
    reply.type = fk_app_ReplyType_REPLY_READINGS;
    reply.liveReadings.arg = (void *)&live_readings_array;

    req.connection()->write(&reply);
    req.connection()->close();

    return true;
}

}
