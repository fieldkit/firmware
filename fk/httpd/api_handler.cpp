#include "httpd/api_handler.h"

namespace fk {

FK_DECLARE_LOGGER("api");

bool send_status(HttpRequest &req) {
    loginfo("handling %s", "QUERY_STATUS");

    fk_app_HttpReply reply = fk_app_HttpReply_init_default;
    reply.type = fk_app_ReplyType_REPLY_STATUS;

    req.connection()->write(&reply);

    return true;
}

bool ApiHandler::handle(HttpRequest &req) {
    auto query = req.query();
    if (query != nullptr) {
        switch (query->type) {
        case fk_app_QueryType_QUERY_STATUS: {
            return send_status(req);
            break;
        }
        default: {
            logerror("unknown query type");
            return false;
        }
        }
    }

    req.connection()->busy("Busy");

    return true;
}

}
