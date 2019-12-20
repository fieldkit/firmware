#include "atlas_api_reply.h"

namespace fk {

AtlasApiReply::AtlasApiReply(Pool &pool) : pool_(&pool) {
    reply_ = fk_atlas_WireAtlasReply_init_default;
}

}
