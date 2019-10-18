#pragma once

#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

enum fk_reset_reason {
    FK_RESET_REASON_POR    = 1,
    FK_RESET_REASON_BOD12  = 2,
    FK_RESET_REASON_BOD33  = 4,
    FK_RESET_REASON_NVM    = 8,
    FK_RESET_REASON_EXT    = 16,
    FK_RESET_REASON_WDT    = 32,
    FK_RESET_REASON_SYST   = 64,
    FK_RESET_REASON_BACKUP = 128
};

void fk_nvm_swap_banks();

int8_t fk_nvm_get_active_bank();

void fk_restart();

enum fk_reset_reason fk_get_reset_reason(void);

const char *fk_get_reset_reason_string();

#ifdef __cplusplus
}
#endif
