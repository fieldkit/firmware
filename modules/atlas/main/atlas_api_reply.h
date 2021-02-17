#pragma once

#include "atlas_protocol.h"
#include "oem_atlas.h"

namespace fk {

class AtlasApiReply {
private:
    Pool *pool_;
    fk_atlas_WireAtlasReply reply_;

public:
    AtlasApiReply(Pool &pool);

public:
    fk_atlas_WireAtlasReply const *reply() {
        return fk_atlas_reply_prepare_encoding(&reply_, pool_);
    }

public:
    void error(const char *message);
    bool has_errors() const;

public:
    bool status_reply(uint8_t const *buffer, size_t size);

};

} // namespace fk
