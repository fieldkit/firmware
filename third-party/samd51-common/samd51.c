#if defined(__SAMD51__)

#include <hpl_flash.h>
#include <hri_pm_d51.h>
#include <hpl_reset.h>

#include "samd51_common.h"

#ifdef __cplusplus
extern "C" {
#endif

void fk_nvm_swap_banks() {
    while (!hri_nvmctrl_get_STATUS_READY_bit(NVMCTRL)) {
        /* Wait until this module isn't busy */
    }

    hri_nvmctrl_write_CTRLB_reg(NVMCTRL, NVMCTRL_CTRLB_CMD_BKSWRST | NVMCTRL_CTRLB_CMDEX_KEY);
}

int8_t fk_nvm_get_active_bank() {
    return hri_nvmctrl_get_STATUS_AFIRST_bit(NVMCTRL);
}

void fk_restart() {
    NVIC_SystemReset();
}

enum fk_reset_reason fk_get_reset_reason(void) {
    return (enum fk_reset_reason)hri_rstc_read_RCAUSE_reg(RSTC);
}

const char *fk_get_reset_reason_string() {
    switch (fk_get_reset_reason()) {
    case FK_RESET_REASON_POR: return "POR";
    case FK_RESET_REASON_BOD12: return "BOD12";
    case FK_RESET_REASON_BOD33: return "BOD33";
    case FK_RESET_REASON_NVM: return "NVM";
    case FK_RESET_REASON_EXT: return "EXT";
    case FK_RESET_REASON_WDT: return "WDT";
    case FK_RESET_REASON_SYST: return "SYST";
    case FK_RESET_REASON_BACKUP: return "BACKUP";
    }
    return "<unknown>";
}

#ifdef __cplusplus
}
#endif

#endif
