#include "httpd/api_handler.h"
#include "protobuf.h"

namespace fk {

FK_DECLARE_LOGGER("api");

bool send_status(HttpRequest &req) {
    loginfo("handling %s", "QUERY_STATUS");

    fk_serial_number_t sn;
    fk_serial_number_get(&sn);

    pb_data_t device_id = {
        .length = sizeof(sn),
        .buffer = &sn,
    };

    fk_app_HttpReply reply = fk_app_HttpReply_init_default;
    reply.type = fk_app_ReplyType_REPLY_STATUS;
    reply.status.identity.deviceId.arg = &device_id;

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
