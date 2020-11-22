#pragma once

#include <loading.h>

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
    bool include_success(uint32_t clock, uint32_t uptime);
    bool include_status(uint32_t clock, uint32_t uptime, bool logs, fkb_header_t const *fkb);
    bool include_readings();
    bool include_scan(NetworkScan scan);
    bool include_listing(const char *path, fk_app_DirectoryEntry *entries, size_t number_entries, size_t total_entries);

public:
    fk_app_HttpReply const *reply() {
        return fk_http_reply_encoding_initialize(&reply_);
    }

};

}
