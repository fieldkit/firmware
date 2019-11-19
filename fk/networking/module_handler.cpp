#include "networking/module_handler.h"

namespace fk {

bool ModuleHandler::handle(HttpServerConnection *connection, Pool &pool) {
    connection->busy(0, "busy");
    return true;
}

}
