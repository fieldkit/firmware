#include <tiny_printf.h>
#include <loading.h>

#include "networking/api_handler.h"
#include "storage/storage.h"
#include "storage/signed_log.h"
#include "protobuf.h"
#include "utilities.h"
#include "records.h"
#include "readings_worker.h"
#include "device_name.h"
#include "state_manager.h"
#include "networking/http_reply.h"
#include "base64.h"

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
    case fk_app_QueryType_QUERY_RECORDING_CONTROL:
    case fk_app_QueryType_QUERY_CONFIGURE: {
        loginfo("handling %s", "QUERY_CONFIGURE");
        return configure(req, query, pool);
    }
    case fk_app_QueryType_QUERY_TAKE_READINGS: {
        loginfo("handling %s", "QUERY_TAKE_READINGS");
        auto worker = create_pool_wrapper<ReadingsWorker, DefaultWorkerPoolSize, PoolWorker<ReadingsWorker>>(true);
        if (!get_ipc()->launch_worker(WorkerCategory::Readings, worker)) {
            delete worker;
            req.connection()->busy("unable to launch");
            return true;
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

static bool flush_configuration(Pool &pool) {
    auto gs = get_global_state_ro();

    StatisticsMemory memory{ MemoryFactory::get_data_memory() };
    Storage storage{ &memory, false };
    if (!storage.begin()) {
        return false;
    }

    auto meta = storage.file(Storage::Meta);
    if (!meta.seek_end()) {
        FK_ASSERT(meta.create());
    }

    fk_serial_number_t sn;

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
    record.identity.name.arg = (void *)gs.get()->general.name;
    record.condition.flags = fk_data_ConditionFlags_CONDITION_FLAGS_NONE;
    if (gs.get()->general.recording) {
        record.condition.flags |= fk_data_ConditionFlags_CONDITION_FLAGS_RECORDING;
    }

    auto srl = SignedRecordLog{ meta };
    if (!srl.append_immutable(SignedRecordKind::State, &record, fk_data_DataRecord_fields, pool)) {
        return false;
    }

    return true;
}

static bool send_retry(HttpRequest &req) {
    req.connection()->busy("storage busy");
    return true;
}

static bool configure(HttpRequest &req, fk_app_HttpQuery *query, Pool &pool) {
    auto lock = storage_mutex.acquire(500);
    if (!lock) {
        return send_retry(req);
    }

    // HACK HACK HACK
    if (query->identity.name.arg != &pool) {
        auto name = (char *)query->identity.name.arg;

        GlobalStateManager gsm;
        gsm.apply([=](GlobalState *gs) {
            strncpy(gs->general.name, name, sizeof(gs->general.name));
        });
    }

    if (query->recording.modifying) {
        GlobalStateManager gsm;
        gsm.apply([=](GlobalState *gs) {
            gs->general.recording = query->recording.enabled;
        });
    }

    if (!flush_configuration(pool)) {
        return false;
    }

    return send_status(req, query, pool);
}

static bool send_status(HttpRequest &req, fk_app_HttpQuery *query, Pool &pool) {
    auto gs = get_global_state_ro();

    HttpReply http_reply{ pool, gs.get() };

    FK_ASSERT(http_reply.include_status());

    req.connection()->write(http_reply.reply());
    req.connection()->close();

    return true;
}

static bool send_readings(HttpRequest &req, fk_app_HttpQuery *query, Pool &pool) {
    auto gs = get_global_state_ro();

    HttpReply http_reply{ pool, gs.get() };

    FK_ASSERT(http_reply.include_status());
    FK_ASSERT(http_reply.include_readings());

    req.connection()->write(http_reply.reply());
    req.connection()->close();

    return true;
}

}
