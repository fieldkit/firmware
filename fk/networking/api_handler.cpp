#include <tiny_printf.h>
#include <loading.h>

#include "networking/api_handler.h"
#include "storage/storage.h"
#include "storage/signed_log.h"
#include "storage/meta_ops.h"
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

static bool send_status(Connection *connection, fk_app_HttpQuery *query, Pool &pool);

static bool send_readings(Connection *connection, fk_app_HttpQuery *query, Pool &pool);

static bool configure(Connection *connection, fk_app_HttpQuery *query, Pool &pool);

bool ApiHandler::handle(Connection *connection, Pool &pool) {
    Reader *reader = connection;
    if (connection->content_type() == WellKnownContentType::TextPlain) {
        reader = new (pool) Base64Reader(reader);
        connection->hex_encoding(true);
    }

    auto query = fk_http_query_prepare_decoding(pool.malloc<fk_app_HttpQuery>(), &pool);
    auto stream = pb_istream_from_readable(reader);
    if (!pb_decode_delimited(&stream, fk_app_HttpQuery_fields, query)) {
        connection->error("error parsing query");
        return true;
    }

    switch (query->type) {
    case fk_app_QueryType_QUERY_STATUS: {
        loginfo("handling %s", "QUERY_STATUS");
        return send_status(connection, query, pool);
    }
    case fk_app_QueryType_QUERY_RECORDING_CONTROL:
    case fk_app_QueryType_QUERY_CONFIGURE: {
        loginfo("handling %s", "QUERY_CONFIGURE");
        return configure(connection, query, pool);
    }
    case fk_app_QueryType_QUERY_TAKE_READINGS: {
        loginfo("handling %s", "QUERY_TAKE_READINGS");
        auto worker = create_pool_wrapper<ReadingsWorker, DefaultWorkerPoolSize, PoolWorker<ReadingsWorker>>(true);
        if (!get_ipc()->launch_worker(WorkerCategory::Readings, worker)) {
            delete worker;
            connection->busy(TenSecondsMs, "unable to launch");
            return true;
        }
        return send_readings(connection, query, pool);
    }
    case fk_app_QueryType_QUERY_GET_READINGS: {
        loginfo("handling %s", "QUERY_GET_READINGS");
        return send_readings(connection, query, pool);
    }
    default: {
        break;
    }
    }

    connection->error("unknown query type");

    return true;
}

static bool flush_configuration(Pool &pool) {
    auto gs = get_global_state_ro();

    StatisticsMemory memory{ MemoryFactory::get_data_memory() };
    Storage storage{ &memory, false };
    if (!storage.begin()) {
        return false;
    }

    MetaOps ops{ storage };
    if (!ops.write_state(gs.get(), pool)) {
        return false;
    }

    return true;
}

static bool configure(Connection *connection, fk_app_HttpQuery *query, Pool &pool) {
    auto lock = storage_mutex.acquire(500);
    if (!lock) {
        return connection->busy(OneSecondMs, "storage busy");
    }

    GlobalStateManager gsm;

    auto name = pb_get_string_if_provided(query->identity.name.arg, pool);
    if (name != nullptr) {
        gsm.apply([=](GlobalState *gs) {
            loginfo("rename: '%s'", name);
            strncpy(gs->general.name, name, sizeof(gs->general.name));
        });
    }

    if (query->recording.modifying) {
        gsm.apply([=](GlobalState *gs) {
            loginfo("recording: %d", query->recording.enabled);
            gs->general.recording = query->recording.enabled;
        });
    }

    if (query->loraSettings.modifying) {
        auto app_key = (pb_data_t *)query->loraSettings.appKey.arg;
        auto app_eui = (pb_data_t *)query->loraSettings.appEui.arg;

        loginfo("lora app key: %s (%zd)", pb_data_to_hex_string(app_key, pool), app_key->length);
        loginfo("lora app eui: %s (%zd)", pb_data_to_hex_string(app_eui, pool), app_eui->length);

        gsm.apply([=](GlobalState *gs) {
            FK_ASSERT(app_key->length == LoraAppKeyLength);
            FK_ASSERT(app_eui->length == LoraAppEuiLength);

            memcpy(gs->lora.app_key, app_key->buffer, LoraAppKeyLength);
            memcpy(gs->lora.app_eui, app_eui->buffer, LoraAppEuiLength);
        });
    }

    if (!flush_configuration(pool)) {
        return false;
    }

    return send_status(connection, query, pool);
}

static bool send_status(Connection *connection, fk_app_HttpQuery *query, Pool &pool) {
    auto gs = get_global_state_ro();

    HttpReply http_reply{ pool, gs.get() };

    FK_ASSERT(http_reply.include_status());

    connection->write(http_reply.reply());
    connection->close();

    return true;
}

static bool send_readings(Connection *connection, fk_app_HttpQuery *query, Pool &pool) {
    auto gs = get_global_state_ro();

    HttpReply http_reply{ pool, gs.get() };

    FK_ASSERT(http_reply.include_status());
    FK_ASSERT(http_reply.include_readings());

    connection->write(http_reply.reply());
    connection->close();

    return true;
}

}
