#include <tiny_printf.h>
#include <loading.h>

#include "networking/api_handler.h"
#include "storage/storage.h"
#include "protobuf.h"
#include "utilities.h"

extern const struct fkb_header_t fkb_header;

namespace fk {

FK_DECLARE_LOGGER("api");

static bool send_status(HttpRequest &req);

static bool configure(HttpRequest &req);

bool ApiHandler::handle(HttpRequest &req) {
    if (req.content_type() != WellKnownContentType::ApplicationFkHttp) {
        req.connection()->error("unexpected content-type");
        return true;
    }

    auto query = req.query();
    if (query == nullptr) {
        req.connection()->error("missing query");
        return true;
    }

    switch (query->type) {
    case fk_app_QueryType_QUERY_STATUS: {
        loginfo("handling %s", "QUERY_STATUS");
        return send_status(req);
    }
    case fk_app_QueryType_QUERY_CONFIGURE: {
        loginfo("handling %s", "QUERY_CONFIGURE");
        return configure(req);
    }
    default: {
        break;
    }
    }

    req.connection()->error("unknown query type");
    return true;
}

static bool configure(HttpRequest &req) {
    return send_status(req);
}

static bool send_status(HttpRequest &req) {
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

    fk_app_HttpReply reply = fk_app_HttpReply_init_default;
    reply.type = fk_app_ReplyType_REPLY_STATUS;
    reply.status.version = 1;
    reply.status.uptime = fk_uptime();
    reply.status.identity.device.arg = (void *)"device";
    reply.status.identity.stream.arg = (void *)"stream";
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
        for (auto file_number : { Storage::Data, Storage::Meta }) {
            auto file = storage.file(file_number);
            if (file.seek(LastRecord)) {
                auto &stream = streams[file_number];
                stream.size = file.size();
                stream.block = file.record();
                reply.status.memory.dataMemoryUsed += stream.size;
            }
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

}
