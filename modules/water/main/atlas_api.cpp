#include "atlas_api.h"

namespace fk {

FK_DECLARE_LOGGER("water");

AtlasApi::AtlasApi(AtlasSensorType type) : type_(type) {
}

bool AtlasApi::handle(HttpServerConnection *connection, Pool &pool) {
    Reader *reader = connection;

    auto query = fk_atlas_query_prepare_decoding(pool.malloc<fk_atlas_WireAtlasQuery>(), &pool);
    auto stream = pb_istream_from_readable(reader);
    if (!pb_decode_delimited(&stream, fk_app_HttpQuery_fields, query)) {
        logwarn("error parsing query (%" PRIu32 ")", connection->length());
        connection->error("error parsing query");
        return true;
    }

    switch (query->calibration.operation) {
    case fk_atlas_CalibrationOperation_CALIBRATION_STATUS: {
        break;
    }
    case fk_atlas_CalibrationOperation_CALIBRATION_CLEAR: {
        break;
    }
    case fk_atlas_CalibrationOperation_CALIBRATION_SET: {
        break;
    }
    default: {
        break;
    }
    }

    AtlasApiReply reply{ pool };

    connection->write(200, "ok", reply.reply(), fk_atlas_WireAtlasReply_fields);

    connection->close();

    return true;
}

}
