#pragma once

#include "data.h"

namespace fk {

template<size_t N>
class NModuleReadings : public ModuleReadings {
private:
    typedef struct ModuleReading {
        float value{ 0.0f };
    } ModuleReading;

    size_t nreadings_{ N };
    ModuleReading readings_[N];

public:
    NModuleReadings() : nreadings_(N) {
    }

    NModuleReadings(size_t size) : nreadings_(size) {
        FK_ASSERT(size <= N);
    }

public:
    size_t size() const override {
        return nreadings_;
    }

    void set(int32_t i, float value) override {
        if ((size_t)i < N) {
            readings_[i].value = value;
        }
    }

    float get(int32_t i) const override {
        return readings_[i].value;
    }

    ModuleReadings *clone(Pool &pool) const override {
        auto clone = new (pool) NModuleReadings<N>();
        clone->nreadings_ = nreadings_;
        memcpy(clone->readings_, readings_, sizeof(readings_));
        return clone;
    }
};

}
