#include "atlas_protocol.h"

namespace fk {


fk_atlas_WireAtlasQuery *fk_atlas_query_prepare_decoding(fk_atlas_WireAtlasQuery *query, Pool *pool) {
    *query = fk_atlas_WireAtlasQuery_init_default;

    return query;
}

fk_atlas_WireAtlasReply *fk_atlas_reply_prepare_encoding(fk_atlas_WireAtlasReply *reply, Pool *pool) {
    if (reply->errors.arg != nullptr) {
        reply->errors.funcs.encode = pb_encode_array;
        auto array = (pb_array_t *)reply->errors.arg;
        for (auto i = 0u; i < array->length; ++i) {
            auto error = &((fk_atlas_Error *)array->buffer)[i];
            error->message.funcs.encode = pb_encode_string;
        }
    }

    return reply;
}

}
