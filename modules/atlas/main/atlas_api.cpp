#include "atlas_api.h"
#include "modules/eeprom.h"

namespace fk {

FK_DECLARE_LOGGER("water");

AtlasApi::AtlasApi(AtlasSensorType type, OemAtlas &atlas) : type_(type), atlas_(&atlas) {
}

bool AtlasApi::handle(ModuleContext mc, HttpServerConnection *connection, Pool &pool) {
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
        status(mc, reply, pool);
        break;
    }
    case fk_atlas_CalibrationOperation_CALIBRATION_CLEAR: {
        clear(mc, reply, pool);
        break;
    }
    case fk_atlas_CalibrationOperation_CALIBRATION_SET: {
        calibrate(mc, reply, query->calibration, pool);
        break;
    }
    default: {
        logwarn("unknown operation (%d)", query->calibration.operation);
        reply.error("unknown operation");
        break;
    }
    }

    return send_reply(connection, pool, reply);
}

bool AtlasApi::send_reply(HttpServerConnection *connection, Pool &pool, AtlasApiReply &reply) {
    if (reply.has_errors()) {
        connection->write(500, "error", reply.reply(), fk_atlas_WireAtlasReply_fields);
    }
    else {
        connection->write(200, "ok", reply.reply(), fk_atlas_WireAtlasReply_fields);
    }

    connection->close();

    return true;
}

bool AtlasApi::status(ModuleContext mc, AtlasApiReply &reply, Pool &pool) {
    auto module_bus = get_board()->i2c_module();
    ModuleEeprom eeprom{ module_bus };

    size_t size = 0;
    auto buffer = (uint8_t *)pool.malloc(MaximumConfigurationSize);
    bzero(buffer, MaximumConfigurationSize);
    if (!eeprom.read_configuration(buffer, size, MaximumConfigurationSize)) {
        logwarn("error reading configuration");
        return false;
    }

    return reply.status_reply(buffer, size);
}

bool AtlasApi::clear(ModuleContext mc, AtlasApiReply &reply, Pool &pool) {
    loginfo("clearing calibration");

    auto module_bus = get_board()->i2c_module();
    ModuleEeprom eeprom{ module_bus };

    if (!eeprom.erase_configuration()) {
        logerror("error clearing module configuration");
    }

    if (!atlas_->wake()) {
        logwarn("error waking (clear)");
        reply.error("error waking (clear)");
        return false;
    }

    if (!atlas_->clear_calibration()) {
        logwarn("error clearing");
        reply.error("error clearing");
        return false;
    }

    return status(mc, reply, pool);
}

bool AtlasApi::calibrate(ModuleContext mc, AtlasApiReply &reply, fk_atlas_AtlasCalibrationCommand command, Pool &pool) {
    if (!clear(mc, reply, pool)) {
        return false;
    }

    auto data = (pb_data_t *)command.configuration.arg;

    loginfo("calibrating (%zd bytes)", data->length);

    auto module_bus = get_board()->i2c_module();
    ModuleEeprom eeprom{ module_bus };

    if (!eeprom.write_configuration((uint8_t *)data->buffer, data->length)) {
        logerror("error writing module configuration");
    }

    return status(mc, reply, pool);
}

} // namespace fk
