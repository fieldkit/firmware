#include <tiny_printf.h>

#include "display/lora_view.h"
#include "state_ref.h"
#include "hal/board.h"
#include "hal/display.h"
#include "platform.h"

#if defined(__SAMD51__)
#include "hal/metal/metal_ipc.h"
#else
#include "hal/linux/linux_ipc.h"
#endif

namespace fk {

void LoraView::tick(ViewController *views, Pool &pool) {
    auto bus = get_board()->i2c_core();
    auto display = get_display();

    auto gs = get_global_state_ro();
    auto &lora = gs.get()->lora;

    tiny_snprintf(status_, sizeof(status_), "rpy %" PRIu32 " txs %" PRIu32 " txf %" PRIu32, lora.tx_confirmed_failures, lora.tx_successes,
                  lora.tx_failures);

    if (!lora.has_module) {
        display->simple(SimpleScreen{ "No Module!", status_ });
        return;
    }

    if (lora.joined > 0) {
        display->simple(SimpleScreen{ "Sending", status_ });
    } else {
        display->simple(SimpleScreen{ "Joining", status_ });
    }
}

void LoraView::hide() {
    get_ipc()->signal_workers(WorkerCategory::Lora, 9);
}

} // namespace fk
