#include "atlas_api.h"

namespace fk {

FK_DECLARE_LOGGER("water");

AtlasApi::AtlasApi(AtlasSensorType type, OemAtlas &atlas) : type_(type), atlas_(&atlas) {
}

bool AtlasApi::handle(HttpServerConnection *connection, Pool &pool) {
    Reader *reader = connection;

    AtlasApiReply reply{ pool };

    auto query = fk_atlas_query_prepare_decoding(pool.malloc<fk_atlas_WireAtlasQuery>(), &pool);
    auto stream = pb_istream_from_readable(reader);
    if (!pb_decode_delimited(&stream, fk_atlas_WireAtlasQuery_fields, query)) {
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

    switch (atlas_->type()) {
    case AtlasSensorType::Ec: {
        reply.status((fk_atlas_EcCalibrations)status.value);
        break;
    }
    case AtlasSensorType::Ph: {
        reply.status((fk_atlas_PhCalibrations)status.value);
        break;
    }
    case AtlasSensorType::Do: {
        reply.status((fk_atlas_DoCalibrations)status.value);
        break;
    }
    case AtlasSensorType::Temp: {
        reply.status((fk_atlas_TempCalibrations)status.value);
        break;
    }
    case AtlasSensorType::Orp: {
        reply.status((fk_atlas_OrpCalibrations)status.value);
        break;
    }
    default: {
        break;
    }
    }

    return true;
}

bool AtlasApi::clear(AtlasApiReply &reply) {
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

bool AtlasApi::set(AtlasApiReply &reply) {
    loginfo("calibration set");

    if (!atlas_->wake()) {
        reply.error("error waking");
        return false;
    }

    return true;
}

} // namespace fk
