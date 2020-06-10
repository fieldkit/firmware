#pragma once

#include "self_check.h"
#include "hal/display.h"

namespace fk {

class DisplaySelfCheckCallbacks : public SelfCheckCallbacks {
public:
    constexpr static size_t NumberOfChecks = 14;

private:
    SelfCheckScreen screen_;
    Check checks_[NumberOfChecks]{ };
    Check *queued_[NumberOfChecks + 1]{ };
    size_t number_{ 0 };

public:
    DisplaySelfCheckCallbacks();

public:
    void update(SelfCheckStatus status) override;
    void append(const char *name, CheckStatus status);
    void clear();

public:
    SelfCheckScreen &screen();

};


}
