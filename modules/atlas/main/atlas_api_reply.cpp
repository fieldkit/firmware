#include "atlas_api_reply.h"

namespace fk {

AtlasApiReply::AtlasApiReply(Pool &pool) : pool_(&pool) {
    reply_ = fk_atlas_WireAtlasReply_init_default;
}

void AtlasApiReply::error(const char *message) {
    fk_atlas_Error errors[] = {
        {
            .type = fk_atlas_ErrorType_GENERAL,
            .message = {
                .funcs = {},
                .arg = (void *)message,
            },
        }
    };

    pb_array_t errors_array = {
        .length = sizeof(errors) / sizeof(fk_atlas_Error),
        .itemSize = sizeof(fk_atlas_Error),
        .buffer = pool_->copy(errors, sizeof(errors)),
        .fields = fk_atlas_Error_fields,
    };

    reply_.type = fk_atlas_ReplyType_REPLY_ERROR;
    reply_.errors.arg = (void *)pool_->copy(&errors_array, sizeof(errors_array));
}

bool AtlasApiReply::has_errors() const {
    return reply_.errors.arg != nullptr;
}

bool AtlasApiReply::status_reply(uint8_t const *buffer, size_t size) {
    reply_.type = fk_atlas_ReplyType_REPLY_STATUS;
    reply_.has_calibration = true;

    if (size > 0) {
        auto cfg_data = pool_->malloc_with<pb_data_t>({
            .length = size,
            .buffer = buffer,
        });

        reply_.calibration.configuration.arg = (void *)cfg_data;
        reply_.calibration.configuration.funcs.encode = pb_encode_data;
    }

    return true;
}

} // namespace fk
