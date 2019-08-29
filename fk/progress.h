#pragma once

#include "common.h"

namespace fk {

class ProgressCallbacks {
public:
    virtual void progress(float progress) = 0;

};

class NoopProgressCallbacks : public ProgressCallbacks {
public:
    void progress(float progress) override {
    }

};

}
