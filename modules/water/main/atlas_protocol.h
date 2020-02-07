#pragma once

#include <fk-atlas-protocol.h>

#include "pool.h"
#include "protobuf.h"

namespace fk {

fk_atlas_WireAtlasQuery *fk_atlas_query_prepare_decoding(fk_atlas_WireAtlasQuery *query, Pool *pool);

fk_atlas_WireAtlasReply *fk_atlas_reply_prepare_encoding(fk_atlas_WireAtlasReply *reply, Pool *pool);

} // namespace fk
