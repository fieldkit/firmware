#pragma once

#include "common.h"
#include "pool.h"
#include "records.h"

namespace fk {

class GlobalState;

class HttpReply {
private:
    Pool *pool_;
    GlobalState const *gs_;
    fk_app_HttpReply reply_;

public:
    HttpReply(Pool &pool, GlobalState const *gs);

public:
    bool include_status();
    bool include_readings();

public:
    fk_app_HttpReply const *reply() {
        return fk_http_reply_encoding_initialize(&reply_);
    }

};

}