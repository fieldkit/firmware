#if !defined(__SAMD51__)

#include "samd51_common.h"

#ifdef __cplusplus
extern "C" {
#endif

void fk_nvm_swap_banks() {
}

int8_t fk_nvm_get_active_bank() {
    return 0;
}

void fk_restart() {
}

enum fk_reset_reason fk_get_reset_reason(void) {
    return (enum fk_reset_reason)FK_RESET_REASON_POR;
}

const char *fk_get_reset_reason_string() {
    return "<unknown>";
}

#ifdef __cplusplus
}
#endif

#endif
