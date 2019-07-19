#include <loading.h>

#include "httpd/api_handler.h"
#include "storage.h"
#include "protobuf.h"
#include "printf.h"

extern const struct fkb_header_t fkb_header;

namespace fk {

FK_DECLARE_LOGGER("api");

constexpr static uint32_t BootloaderSize = 0x4000;

bool send_status(HttpRequest &req) {
    loginfo("handling %s", "QUERY_STATUS");

    StatisticsMemory memory{ MemoryFactory::get_data_memory() };
    Storage storage{ &memory };

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
    // reply.status.hardware
    reply.status.power.battery.voltage = 4000;
    reply.status.power.battery.percentage = 45;

    reply.status.memory.sramAvailable = fk_free_memory();
    reply.status.memory.programAvailable = 1024 * 1024 - BootloaderSize - fkb_header.firmware.binary_size;
    reply.status.memory.extendedMemoryAvailable = 8 * 1024 * 1024;
    reply.status.memory.dataMemoryAvailable = 0;
    reply.status.memory.dataMemoryUsed = 0;

    reply.status.gps.fix = true;
    reply.status.gps.time = 0;
    reply.status.gps.satellites = 3;
    reply.status.gps.longitude = 0.0f;
    reply.status.gps.latitude = 0.0f;
    reply.status.gps.altitude = 0.0f;

    auto file = storage.file(0);

    FK_ASSERT(file.seek(LastRecord));

    fk_app_DataStream streams[] = {
        {
            .id = 1,
            .time = 0,
            .size = file.size(),
            .version = 0,
            .block = file.record(),
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
                .arg = (void *)"/fk/v1/download",
            },
        },
    };

    pb_array_t streams_array = {
        .length = (size_t)1,
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

    return true;
}

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
        return send_status(req);
    }
    default: {
        break;
    }
    }

    req.connection()->error("unknown query type");
    return true;
}

}
