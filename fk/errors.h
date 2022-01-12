#pragma once

#include "common.h"

namespace fk {

enum class Error : int32_t {
    None = 0,
    General = -1,
    Bus = -2,
    TooSoon = -3,
    IO = -4,
    EoF = -5,
};

struct FaultCode {
    const char *code;
    const char *message;
};

extern FaultCode BatteryGaugeFailure;

int32_t fk_fault_set(FaultCode *code);

FaultCode *fk_fault_get();

} // namespace fk
