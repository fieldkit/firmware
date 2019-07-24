#include "networking/module_handler.h"

namespace fk {

bool ModuleHandler::handle(HttpRequest &req) {
    auto query = req.query();
    if (query != nullptr) {
        switch (query->type) {
        default: {
            break;
        }
        }
    }

    req.connection()->busy("Busy");

    return true;
}

}
