#include "atlas_api.h"

namespace fk {

FK_DECLARE_LOGGER("water");

AtlasApi::AtlasApi(AtlasSensorType type, OemAtlas &atlas) : type_(type), atlas_(&atlas) {
}

bool AtlasApi::handle(HttpServerConnection *connection, Pool &pool) {
    Reader *reader = connection;

    AtlasApiReply reply{ pool };

    if (connection->length() == 0) {
        connection->error(500, "invalid query");
        return true;
    }

    auto query = fk_atlas_query_prepare_decoding(pool.malloc<fk_atlas_WireAtlasQuery>(), &pool);
    auto stream = pb_istream_from_readable(reader);
    if (!pb_decode_delimited(&stream, fk_atlas_WireAtlasQuery_fields, query)) {
        logwarn("error parsing query (%" PRIu32 ")", connection->length());
        reply.error("error parsing query");
        return true;
    }

    loginfo("operation (%d)", query->calibration.operation);

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
        calibrate(reply, query->calibration);
        break;
    }
    default: {
        logwarn("unknown operation (%d)", query->calibration.operation);
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
    if (!atlas_->wake()) {
        reply.error("error waking");
        return false;
    }

    auto status = atlas_->calibration();
    if (!status.success) {
        return false;
    }

    return reply.status_reply(*atlas_, status);
}

bool AtlasApi::clear(AtlasApiReply &reply) {
    loginfo("clearing calibration");

    if (!atlas_->wake()) {
        reply.error("error waking");
        return false;
    }

    if (!atlas_->clear_calibration()) {
        reply.error("error clearing");
        return false;
    }

    loginfo("cleared calibration");

    return true;
}

bool AtlasApi::calibrate(AtlasApiReply &reply, fk_atlas_AtlasCalibrationCommand command) {
    if (!atlas_->wake()) {
        reply.error("error waking");
        return false;
    }

    uint8_t which = command.which;

    if (false) {
        switch (atlas_->type()) {
        case AtlasSensorType::Ec: {
            which = (uint8_t)command.ec;
            break;
        }
        case AtlasSensorType::Ph: {
            which = (uint8_t)command.ph;
            break;
        }
        case AtlasSensorType::Do: {
            which = (uint8_t)command.dissolvedOxygen;
            break;
        }
        case AtlasSensorType::Temp: {
            which = (uint8_t)command.temp;
            break;
        }
        case AtlasSensorType::Orp: {
            which = (uint8_t)command.orp;
            break;
        }
        default: {
            break;
        }
        }
    }

    loginfo("calibrating (0x%x @ %f) ...", which, command.value);

    auto status = atlas_->calibrate(which, command.value);
    if (!status.success) {
        reply.error("error calibrating");
        return false;
    }

    return reply.status_reply(*atlas_, status);
}

} // namespace fk
