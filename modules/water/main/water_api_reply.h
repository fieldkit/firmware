#pragma once

#include "records.h"

namespace fk {

class WaterApiReply {
private:
    Pool *pool_;
    fk_app_ModuleHttpReply reply_;

public:
    WaterApiReply(Pool &pool);

public:
    fk_app_ModuleHttpReply const *reply() {
        return fk_module_reply_prepare_encoding(&reply_, pool_);
    }

public:
    void error(const char *message);
    bool has_errors() const;

public:
    bool status_reply(uint8_t const *buffer, size_t size);

};

} // namespace fk
