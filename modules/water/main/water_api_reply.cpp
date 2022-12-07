#include "water_api_reply.h"

namespace fk {

WaterApiReply::WaterApiReply(Pool &pool) : pool_(&pool) {
    reply_ = fk_app_ModuleHttpReply_init_default;
}

void WaterApiReply::error(const char *message) {
    fk_app_Error errors[] = {
        {
            .message = {
                .funcs = {},
                .arg = (void *)message,
            },
        }
    };

    pb_array_t errors_array = {
        .length = sizeof(errors) / sizeof(fk_app_Error),
        .allocated = sizeof(errors) / sizeof(fk_app_Error),
        .item_size = sizeof(fk_app_Error),
        .buffer = pool_->copy(errors, sizeof(errors)),
        .fields = fk_app_Error_fields,
    };

    reply_.type = fk_app_ModuleReplyType_MODULE_REPLY_ERROR;
    reply_.errors.arg = (void *)pool_->copy(&errors_array, sizeof(errors_array));
}

bool WaterApiReply::has_errors() const {
    return reply_.errors.arg != nullptr;
}

bool WaterApiReply::status_reply(uint8_t const *buffer, size_t size) {
    reply_.type = fk_app_ModuleReplyType_MODULE_REPLY_SUCCESS;

    if (size > 0 && buffer != nullptr) {
        auto cfg_data = pool_->malloc_with<pb_data_t>({
            .length = size,
            .buffer = buffer,
        });

        reply_.configuration.arg = (void *)cfg_data;
        reply_.configuration.funcs.encode = pb_encode_data;
    }

    return true;
}

} // namespace fk
