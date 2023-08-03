#include "water_api.h"
#include "water_api_reply.h"
#include "modules/eeprom.h"
#include "records.h"

namespace fk {

FK_DECLARE_LOGGER("water");

WaterApi::WaterApi() {
}

bool WaterApi::handle(ModuleContext mc, HttpServerConnection *connection, Pool &pool) {
    Reader *reader = connection;

    WaterApiReply reply{ pool };

    if (connection->length() == 0) {
        connection->error(HttpStatus::ServerError, "invalid query", pool);
        return true;
    }

    auto query = fk_module_query_prepare_decoding(pool.malloc<fk_app_ModuleHttpQuery>(), &pool);
    auto stream = pb_istream_from_readable(reader);
    if (!pb_decode_delimited(&stream, fk_app_ModuleHttpQuery_fields, query)) {
        logwarn("error parsing query (%" PRIu32 ")", connection->length());
        reply.error("error parsing query");
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

bool WaterApi::send_reply(HttpStatus status_code, HttpServerConnection *connection, WaterApiReply &reply, Pool &pool) {
    if (reply.has_errors()) {
        connection->write(status_code, "error", reply.reply(), fk_app_ModuleHttpReply_fields, pool);
    } else {
        connection->write(status_code, "ok", reply.reply(), fk_app_ModuleHttpReply_fields, pool);
    }

    connection->close();

    return true;
}

bool WaterApi::status(ModuleContext mc, WaterApiReply &reply, Pool &pool) {
    auto module_bus = get_board()->i2c_module();
    ModuleEeprom eeprom{ module_bus };

    size_t size = 0u;
    uint8_t *buffer = nullptr;
    if (!eeprom.read_configuration(&buffer, size, &pool)) {
        logwarn("reading configuration");
        return false;
    }

    return reply.status_reply(buffer, size);
}

bool WaterApi::clear(ModuleContext mc, WaterApiReply &reply, Pool &pool) {
    loginfo("clearing configuration");

    auto module_bus = get_board()->i2c_module();
    ModuleEeprom eeprom{ module_bus };

    if (!eeprom.erase_configuration(EEPROM_ADDRESS_CONFIG_SIZE)) {
        logerror("clearing module configuration");
    }

    return status(mc, reply, pool);
}

bool WaterApi::calibrate(ModuleContext mc, WaterApiReply &reply, fk_app_ModuleHttpQuery *query, Pool &pool) {
    auto data = (pb_data_t *)query->configuration.arg;

    loginfo("configuring (%zd bytes)", data->length);

    auto module_bus = get_board()->i2c_module();
    ModuleEeprom eeprom{ module_bus };

    if (!eeprom.erase_configuration(data->length)) {
        logerror("clearing module configuration");
    }

    if (!eeprom.write_configuration((uint8_t *)data->buffer, data->length)) {
        logerror("writing module configuration");
    }

    return status(mc, reply, pool);
}

} // namespace fk
