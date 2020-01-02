#include "atlas_api.h"

namespace fk {

FK_DECLARE_LOGGER("water");

AtlasApi::AtlasApi(AtlasSensorType type) : type_(type) {
}

bool AtlasApi::handle(HttpServerConnection *connection, Pool &pool) {
    Reader *reader = connection;

    AtlasApiReply reply{ pool };

    auto query = fk_atlas_query_prepare_decoding(pool.malloc<fk_atlas_WireAtlasQuery>(), &pool);
    auto stream = pb_istream_from_readable(reader);
    if (!pb_decode_delimited(&stream, fk_app_HttpQuery_fields, query)) {
        logwarn("error parsing query (%" PRIu32 ")", connection->length());
        reply.error("error parsing query");
        return true;
    }

    switch (query->calibration.operation) {
    case fk_atlas_CalibrationOperation_CALIBRATION_STATUS: {
        status(reply);
        break;
    }
    case fk_atlas_CalibrationOperation_CALIBRATION_CLEAR: {
        clear(reply);
        break;
    }
    case fk_atlas_CalibrationOperation_CALIBRATION_SET: {
        set(reply);
        break;
    }
    default: {
        logwarn("unknown operation");
        reply.error("unknown operation");
        break;
    }
    }

    return handle(connection, pool, reply);
}

bool AtlasApi::handle(HttpServerConnection *connection, Pool &pool, AtlasApiReply &reply) {
    if (reply.has_errors()) {
        connection->write(500, "error", reply.reply(), fk_atlas_WireAtlasReply_fields);
    }
    else {
        connection->write(200, "ok", reply.reply(), fk_atlas_WireAtlasReply_fields);
    }

    connection->close();

    return true;
}

bool AtlasApi::status(AtlasApiReply &reply) {
    loginfo("calibration status");

    return true;
}

bool AtlasApi::clear(AtlasApiReply &reply) {
    loginfo("calibration clear");

    return true;
}

bool AtlasApi::set(AtlasApiReply &reply) {
    loginfo("calibration set");

    return true;
}

} // namespace fk
