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
        reply_.calibration.temp = s;
    }

    void status(fk_atlas_DoCalibrations s) {
        reply_.calibration.dissolvedOxygen = s;
    }

    void status(fk_atlas_PhCalibrations s) {
        reply_.calibration.ph = s;
    }

    void status(fk_atlas_EcCalibrations s) {
        reply_.calibration.ec = s;
    }

    void status(fk_atlas_OrpCalibrations s) {
        reply_.calibration.orp = s;
    }

    void status(uint8_t s) {
        reply_.calibration.raw = s;
    }

};

}
