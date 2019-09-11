#include "networking/module_handler.h"

namespace fk {

bool ModuleHandler::handle(Connection *connection, Pool &pool) {
    connection->busy("Busy");
    return true;
}

}
