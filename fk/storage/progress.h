#pragma once

#include "common.h"

namespace fk {

class ProgressTracker {
private:
    const char *facility_;
    const char *prefix_;
    uint32_t total_;
    uint32_t started_{ 0 };
    uint32_t bytes_{ 0 };
    uint32_t status_{ 0 };

public:
    ProgressTracker(const char *facility, const char *prefix, uint32_t total);

public:
    void update(int32_t bytes);

    bool busy() const;

    bool done() const;

    uint32_t elapsed() const;

    uint32_t bytes() const;

    uint32_t remaining_bytes() const;
};

}
