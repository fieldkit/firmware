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
#include "clock.h"

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
        logwarn("error parsing query (%" PRIu32 ")", connection->length());
        connection->error("error parsing query");
        return true;
    }

    switch (query->type) {
    case fk_app_QueryType_QUERY_STATUS: {
        loginfo("handling %s", "QUERY_STATUS");
        return send_status(connection, query, pool);
    }
    case fk_app_QueryType_QUERY_RECORDING_CONTROL: {
        loginfo("handling %s", "QUERY_RECORDING_CONTROL");
        return configure(connection, query, pool);
    }
    case fk_app_QueryType_QUERY_CONFIGURE: {
        loginfo("handling %s", "QUERY_CONFIGURE");
        return configure(connection, query, pool);
    }
    case fk_app_QueryType_QUERY_TAKE_READINGS: {
        loginfo("handling %s", "QUERY_TAKE_READINGS");
        auto worker = create_pool_worker<ReadingsWorker>(true);
        get_ipc()->launch_worker(WorkerCategory::Readings, worker);
        return send_readings(connection, query, pool);
    }
    case fk_app_QueryType_QUERY_GET_READINGS: {
        loginfo("handling %s", "QUERY_GET_READINGS");
        return send_readings(connection, query, pool);
    }
    default: {
        loginfo("unknown %d", query->type);
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
            if (query->recording.enabled) {
                gs->general.recording = get_clock_now();
            }
            else {
                gs->general.recording = 0;
            }
            loginfo("recording: %" PRIu32, gs->general.recording);
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

    auto networks_array = (pb_array_t *)query->networkSettings.networks.arg;
    if (networks_array->length > 0) {
        auto nnetworks = std::min(networks_array->length, MaximumNumberOfWifiNetworks);
        gsm.apply([=](GlobalState *gs) {
            auto networks = (fk_app_NetworkInfo *)networks_array->buffer;
            for (auto i = 0u; i < nnetworks; ++i) {
                auto &n = networks[i];
                auto ssid = (const char *)n.ssid.arg;
                auto password = (const char *)n.password.arg;
                loginfo("[%d] network: %s", i, ssid);

                auto &nc = gs->network.config.wifi_networks[i];
                strncpy(nc.ssid, ssid, sizeof(nc.ssid));
                strncpy(nc.password, password, sizeof(nc.password));
                nc.valid = nc.ssid[0] != 0;
                nc.create = false;
            }
        });
    }

    if (query->schedules.modifying) {
        gsm.apply([=](GlobalState *gs) {
            gs->scheduler.readings.interval = query->schedules.readings.interval;
            gs->scheduler.network.interval = query->schedules.network.interval;
            gs->scheduler.gps.interval = query->schedules.gps.interval;
            gs->scheduler.lora.interval = query->schedules.lora.interval;
            gs->scheduler.readings.cron = lwcron::CronSpec::interval(gs->scheduler.readings.interval);
            gs->scheduler.network.cron = lwcron::CronSpec::interval(gs->scheduler.network.interval);
            gs->scheduler.gps.cron = lwcron::CronSpec::interval(gs->scheduler.gps.interval);
            gs->scheduler.lora.cron = lwcron::CronSpec::interval(gs->scheduler.lora.interval);
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
