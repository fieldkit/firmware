#include "atlas_api.h"
#include "atlas_api_reply.h"
#include "modules/eeprom.h"
#include "records.h"

namespace fk {

FK_DECLARE_LOGGER("water");

AtlasApi::AtlasApi(AtlasSensorType type, OemAtlas &atlas) : type_(type), atlas_(&atlas) {
}

bool AtlasApi::handle(ModuleContext mc, HttpServerConnection *connection, Pool &pool) {
    Reader *reader = connection;

    AtlasApiReply reply{ pool };

    if (connection->length() == 0) {
        connection->error(HttpStatus::ServerError, "invalid query", pool);
        return true;
    }

    auto query = fk_module_query_prepare_decoding(pool.malloc<fk_app_ModuleHttpQuery>(), &pool);
    auto stream = pb_istream_from_readable(reader);
    if (!pb_decode_delimited(&stream, fk_app_ModuleHttpQuery_fields, query)) {
        logwarn("parsing query (%" PRIu32 ")", connection->length());
        reply.error("parsing query");
        return send_reply(HttpStatus::BadRequest, connection, reply, pool);
    }

    switch (query->type) {
    case fk_app_ModuleQueryType_MODULE_QUERY_STATUS: {
        status(mc, reply, pool);
        break;
    }
    case fk_app_ModuleQueryType_MODULE_QUERY_RESET: {
        clear(mc, reply, pool);
        break;
    }
    case fk_app_ModuleQueryType_MODULE_QUERY_CONFIGURE: {
        calibrate(mc, reply, query, pool);
        break;
    }
    default: {
        logwarn("unknown query (%d)", query->type);
        reply.error("unknown query");
        break;
    }
    }

    return send_reply(HttpStatus::Ok, connection, reply, pool);
}

bool AtlasApi::send_reply(HttpStatus status_code, HttpServerConnection *connection, AtlasApiReply &reply, Pool &pool) {
    if (reply.has_errors()) {
        connection->write(status_code, "error", reply.reply(), fk_app_ModuleHttpReply_fields, pool);
    } else {
        connection->write(status_code, "ok", reply.reply(), fk_app_ModuleHttpReply_fields, pool);
    }

    connection->close();

    return true;
}

bool AtlasApi::status(ModuleContext mc, AtlasApiReply &reply, Pool &pool) {
    auto module_bus = get_board()->i2c_module();
    ModuleEeprom eeprom{ module_bus };

    size_t size = 0;
    uint8_t *buffer = nullptr;
    if (!eeprom.read_configuration(&buffer, size, &pool)) {
        logwarn("reading configuration");
        return false;
    }

    return reply.status_reply(buffer, size);
}

bool AtlasApi::clear(ModuleContext mc, AtlasApiReply &reply, Pool &pool) {
    loginfo("clearing configuration");

    auto module_bus = get_board()->i2c_module();
    ModuleEeprom eeprom{ module_bus };

    if (!eeprom.erase_configuration(EEPROM_ADDRESS_CONFIG_SIZE)) {
        logerror("clearing module configuration");
    }

    if (!atlas_->wake()) {
        logwarn("waking (clear)");
        reply.error("waking (clear)");
        return false;
    }

    if (!atlas_->clear_calibration()) {
        logwarn("clearing");
        reply.error("clearing");
        return false;
    }

    return status(mc, reply, pool);
}

bool AtlasApi::calibrate(ModuleContext mc, AtlasApiReply &reply, fk_app_ModuleHttpQuery *query, Pool &pool) {
    if (!clear(mc, reply, pool)) {
        return false;
    }

    auto data = (pb_data_t *)query->configuration.arg;

    loginfo("configuring (%zd bytes)", data->length);

    auto module_bus = get_board()->i2c_module();
    ModuleEeprom eeprom{ module_bus };

    if (!eeprom.write_configuration((uint8_t *)data->buffer, data->length)) {
        logerror("writing module configuration");
    }

    return status(mc, reply, pool);
}

} // namespace fk
