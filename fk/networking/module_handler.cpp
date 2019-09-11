#include "networking/module_handler.h"

namespace fk {

bool ModuleHandler::handle(Connection *connection, Pool &pool) {
    connection->busy(0, "busy");
    return true;
}

}
