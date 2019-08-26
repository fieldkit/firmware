#include "networking/module_handler.h"

namespace fk {

bool ModuleHandler::handle(HttpRequest &req, Pool &pool) {
    req.connection()->busy("Busy");
    return true;
}

}
