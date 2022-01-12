#include "errors.h"

namespace fk {

FaultCode BatteryGaugeFailure = { "F-1000", "Fault: Module Bus" };

FaultCode *fault_code = nullptr;

static inline bool atomic_compare_exchange(FaultCode **ptr, FaultCode *compare, FaultCode *exchange) {
    return __atomic_compare_exchange_n(ptr, &compare, exchange, 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
}

int32_t fk_fault_set(FaultCode *code) {
    atomic_compare_exchange(&fault_code, nullptr, code);
    return 0;
}

FaultCode *fk_fault_get() {
    return fault_code;
}

} // namespace fk