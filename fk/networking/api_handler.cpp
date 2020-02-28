#include <tiny_printf.h>

#include "networking/api_handler.h"
#include "networking/http_reply.h"

#include "storage/storage.h"
#include "storage/signed_log.h"
#include "storage/meta_ops.h"
#include "state_manager.h"
#include "utilities.h"
#include "records.h"
#include "base64.h"
#include "clock.h"

#include "readings_worker.h"
#include "simple_workers.h"

extern const struct fkb_header_t fkb_header;

namespace fk {

FK_DECLARE_LOGGER("api");

static bool send_simple_success(HttpServerConnection *connection, fk_app_HttpQuery *query, Pool &pool);

static bool send_status(HttpServerConnection *connection, fk_app_HttpQuery *query, Pool &pool);

static bool send_readings(HttpServerConnection *connection, fk_app_HttpQuery *query, Pool &pool);

static bool configure(HttpServerConnection *connection, fk_app_HttpQuery *query, Pool &pool);

void ApiHandler::adjust_time_if_necessary(fk_app_HttpQuery const *query) {
    if (query->time > 0) {
        uint32_t difference = std::abs((long)(get_clock_now() - query->time));
        if (difference > AcceptableTimeDriftSeconds) {
            clock_adjust(query->time);
        }
        else {
            logdebug("query time is close enough");
        }
    }
    else {
        logdebug("query time is missing");
    }
}

bool ApiHandler::handle(HttpServerConnection *connection, Pool &pool) {
    Reader *reader = connection;
    if (connection->content_type() == WellKnownContentType::TextPlain) {
        reader = new (pool) Base64Reader(reader);
        connection->hex_encoding(true);
    }

    size_t buffer_size = 256u;
    auto ptr = reinterpret_cast<uint8_t *>(pool.malloc(buffer_size));
    bzero(ptr, buffer_size);
    BufferedReader buffered{ reader, ptr, buffer_size };

    auto query = fk_http_query_prepare_decoding(pool.malloc<fk_app_HttpQuery>(), &pool);
    auto stream = pb_istream_from_readable(&buffered);
    if (!pb_decode_delimited(&stream, fk_app_HttpQuery_fields, query)) {
        fk_dump_memory("NOPARSE ", ptr, 256);
        logwarn("error parsing query (%" PRIu32 ")", connection->length());
        connection->error("error parsing query");
        return true;
    }

    adjust_time_if_necessary(query);

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
        auto worker = create_pool_worker<ReadingsWorker>(false, true);
        get_ipc()->launch_worker(WorkerCategory::Readings, worker);
        return send_readings(connection, query, pool);
    }
    case fk_app_QueryType_QUERY_GET_READINGS: {
        loginfo("handling %s", "QUERY_GET_READINGS");
        return send_readings(connection, query, pool);
    }
    case fk_app_QueryType_QUERY_RESET: {
        loginfo("handling %s", "QUERY_RESET");
        get_ipc()->launch_worker(create_pool_worker<FactoryWipeWorker>());
        return send_simple_success(connection, query, pool);
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
    return gs.get()->flush(pool);
}

static bool configure(HttpServerConnection *connection, fk_app_HttpQuery *query, Pool &pool) {
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
        gsm.apply([&](GlobalState *gs) {
            auto app_key = pb_get_data_if_provided(query->loraSettings.appKey.arg, pool);
            auto app_eui = pb_get_data_if_provided(query->loraSettings.appEui.arg, pool);
            auto app_session_key = pb_get_data_if_provided(query->loraSettings.appSessionKey.arg, pool);
            auto network_session_key = pb_get_data_if_provided(query->loraSettings.networkSessionKey.arg, pool);
            auto device_address = pb_get_data_if_provided(query->loraSettings.deviceAddress.arg, pool);

            if (app_eui != nullptr) {
                loginfo("lora app eui: %s (%zd)", pb_data_to_hex_string(app_eui, pool), app_eui->length);
                FK_ASSERT(app_eui->length == LoraAppEuiLength);
                memcpy(gs->lora.app_eui, app_eui->buffer, LoraAppEuiLength);
            }

            if (app_key != nullptr) {
                loginfo("lora app key: %s (%zd)", pb_data_to_hex_string(app_key, pool), app_key->length);
                FK_ASSERT(app_key->length == LoraAppKeyLength);
                memcpy(gs->lora.app_key, app_key->buffer, LoraAppKeyLength);
            }

            if (app_session_key != nullptr) {
                loginfo("lora app session key: %s (%zd)", pb_data_to_hex_string(app_session_key, pool), app_session_key->length);
                FK_ASSERT(app_session_key->length == LoraAppSessionKeyLength);
                memcpy(gs->lora.app_session_key, app_session_key->buffer, LoraAppSessionKeyLength);
            }

            if (network_session_key != nullptr) {
                loginfo("lora network session key: %s (%zd)", pb_data_to_hex_string(network_session_key, pool), network_session_key->length);
                FK_ASSERT(network_session_key->length == LoraNetworkSessionKeyLength);
                memcpy(gs->lora.network_session_key, network_session_key->buffer, LoraNetworkSessionKeyLength);
            }

            if (device_address != nullptr) {
                loginfo("lora device address: %s (%zd)", pb_data_to_hex_string(device_address, pool), device_address->length);
                FK_ASSERT(device_address->length == LoraDeviceAddressLength);
                memcpy(gs->lora.device_address, device_address->buffer, LoraDeviceAddressLength);
            }
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
            gs->scheduler.readings = query->schedules.readings;
            gs->scheduler.network = query->schedules.network;
            gs->scheduler.gps = query->schedules.gps;
            gs->scheduler.lora = query->schedules.lora;
        });
    }

    if (query->transmission.wifi.modifying) {
        gsm.apply([&](GlobalState *gs) {
            auto url = pb_get_string_if_provided(query->transmission.wifi.url.arg, pool);
            if (url != nullptr) {
                strncpy(gs->transmission.url, url, sizeof(gs->transmission.url));
                loginfo("transmission url: %s", gs->transmission.url);
            }
            else {
                gs->transmission.url[0] = 0;
            }

            auto token = pb_get_string_if_provided(query->transmission.wifi.token.arg, pool);
            if (token != nullptr) {
                strncpy(gs->transmission.token, token, sizeof(gs->transmission.token));
                loginfo("transmission token: %s", gs->transmission.token);
            }
            else {
                gs->transmission.token[0] = 0;
            }
        });
    }

    if (!flush_configuration(pool)) {
        return false;
    }

    return send_status(connection, query, pool);
}

static bool send_simple_success(HttpServerConnection *connection, fk_app_HttpQuery *query, Pool &pool) {
    auto gs = get_global_state_ro();

    HttpReply http_reply{ pool, gs.get() };

    FK_ASSERT(http_reply.include_success(get_clock_now(), fk_uptime()));

    connection->write(http_reply.reply());
    connection->close();

    return true;
}

static bool send_status(HttpServerConnection *connection, fk_app_HttpQuery *query, Pool &pool) {
    auto gs = get_global_state_ro();

    HttpReply http_reply{ pool, gs.get() };

    FK_ASSERT(http_reply.include_status(get_clock_now(), fk_uptime(), &fkb_header));

    connection->write(http_reply.reply());
    connection->close();

    return true;
}

static bool send_readings(HttpServerConnection *connection, fk_app_HttpQuery *query, Pool &pool) {
    auto gs = get_global_state_ro();

    HttpReply http_reply{ pool, gs.get() };

    FK_ASSERT(http_reply.include_status(get_clock_now(), fk_uptime(), &fkb_header));
    FK_ASSERT(http_reply.include_readings());

    connection->write(http_reply.reply());
    connection->close();

    return true;
}

}
