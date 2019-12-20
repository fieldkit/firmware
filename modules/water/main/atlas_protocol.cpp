#include "atlas_protocol.h"

namespace fk {


fk_atlas_WireAtlasQuery *fk_atlas_query_prepare_decoding(fk_atlas_WireAtlasQuery *query, Pool *pool) {
    *query = fk_atlas_WireAtlasQuery_init_default;

    return query;
}

fk_atlas_WireAtlasReply *fk_atlas_reply_prepare_encoding(fk_atlas_WireAtlasReply *reply, Pool *pool) {
    return reply;
}

}
