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

bool AtlasApiReply::status_reply(OemAtlas &atlas, CalibrationStatus calibrationStatus) {
    switch (atlas.type()) {
    case AtlasSensorType::Ec: {
        status((fk_atlas_EcCalibrations)calibrationStatus.value);
        break;
    }
    case AtlasSensorType::Ph: {
        status((fk_atlas_PhCalibrations)calibrationStatus.value);
        break;
    }
    case AtlasSensorType::Do: {
        status((fk_atlas_DoCalibrations)calibrationStatus.value);
        break;
    }
    case AtlasSensorType::Temp: {
        status((fk_atlas_TempCalibrations)calibrationStatus.value);
        break;
    }
    case AtlasSensorType::Orp: {
        status((fk_atlas_OrpCalibrations)calibrationStatus.value);
        break;
    }
    default: {
        break;
    }
    }

    return true;
}


} // namespace fk
