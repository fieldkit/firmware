#pragma once

#include "common.h"

namespace fk {

enum class Operation {
    None,
    Download,
    Upload,
    Fsck,
    Wipe,
    Exporting,
};

class ProgressCallbacks {
public:
    virtual void progress(Operation op, float progress) = 0;
};

class NoopProgressCallbacks : public ProgressCallbacks {
public:
    void progress(Operation op, float progress) override {
    }
};

} // namespace fk
