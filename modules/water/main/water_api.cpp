#include "water_api.h"
#include "modules/eeprom.h"

namespace fk {

FK_DECLARE_LOGGER("water");

WaterApi::WaterApi() {
}

bool WaterApi::handle(ModuleContext mc, HttpServerConnection *connection, Pool &pool) {
    auto module_bus = get_board()->i2c_module();
    ModuleEeprom eeprom{ module_bus };

    auto content_length = connection->length();

    if (content_length > MaximumConfigurationSize) {
        connection->error(HttpStatus::BadRequest, "bad request", pool);
        return true;
    }

    auto configuration = (uint8_t *)pool.malloc(MaximumConfigurationSize);

    if (content_length > 0) {
        loginfo("configuring (%zd bytes)", content_length);

        auto nread = connection->read(configuration, content_length);
        if (nread != (int32_t)content_length) {
            connection->error(HttpStatus::BadRequest, "bad request", pool);
            return true;
        }

        if (!eeprom.write_configuration(configuration, content_length)) {
            logerror("error writing module configuration");
            connection->error(HttpStatus::ServerError, "server error", pool);
            return true;
        }
    }

    size_t size = 0;
    bzero(configuration, MaximumConfigurationSize);
    if (!eeprom.read_configuration(configuration, size, MaximumConfigurationSize)) {
        logwarn("error reading configuration");
        connection->error(HttpStatus::ServerError, "server error", pool);
        return true;
    }

    connection->write(HttpStatus::Ok, "ok", configuration, size, pool);

    return true;
}

} // namespace fk
