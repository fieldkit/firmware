#pragma once

#include "atlas_protocol.h"

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
    void status(fk_atlas_TempCalibrations s) {
        reply_.type = fk_atlas_ReplyType_REPLY_STATUS;
        reply_.has_calibration = true;
        reply_.calibration.type = fk_atlas_SensorType_SENSOR_TEMP;
        reply_.calibration.temp = s;
        reply_.calibration.raw = s;
    }

    void status(fk_atlas_DoCalibrations s) {
        reply_.type = fk_atlas_ReplyType_REPLY_STATUS;
        reply_.has_calibration = true;
        reply_.calibration.type = fk_atlas_SensorType_SENSOR_DO;
        reply_.calibration.dissolvedOxygen = s;
        reply_.calibration.raw = s;
    }

    void status(fk_atlas_PhCalibrations s) {
        reply_.type = fk_atlas_ReplyType_REPLY_STATUS;
        reply_.has_calibration = true;
        reply_.calibration.type = fk_atlas_SensorType_SENSOR_PH;
        reply_.calibration.ph = s;
        reply_.calibration.raw = s;
    }

    void status(fk_atlas_EcCalibrations s) {
        reply_.type = fk_atlas_ReplyType_REPLY_STATUS;
        reply_.has_calibration = true;
        reply_.calibration.type = fk_atlas_SensorType_SENSOR_EC;
        reply_.calibration.ec = s;
        reply_.calibration.raw = s;
    }

    void status(fk_atlas_OrpCalibrations s) {
        reply_.type = fk_atlas_ReplyType_REPLY_STATUS;
        reply_.has_calibration = true;
        reply_.calibration.type = fk_atlas_SensorType_SENSOR_ORP;
        reply_.calibration.orp = s;
        reply_.calibration.raw = s;
    }

};

} // namespace fk
