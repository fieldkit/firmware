#include "water_api.h"
#include "modules/eeprom.h"

namespace fk {

FK_DECLARE_LOGGER("water");

WaterApi::WaterApi() {
}

bool WaterApi::handle(ModuleContext mc, HttpServerConnection *connection, Pool &pool) {
    connection->error(500, "invalid query", pool);
    return true;
}

} // namespace fk
