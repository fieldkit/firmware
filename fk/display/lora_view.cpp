#include <tiny_printf.h>

#include "display/lora_view.h"
#include "state_ref.h"
#include "hal/board.h"
#include "hal/display.h"
#include "platform.h"

namespace fk {

void LoraView::tick(ViewController *views, Pool &pool) {
    auto bus = get_board()->i2c_core();
    auto display = get_display();

    auto gs = get_global_state_ro();
    auto &lora = gs.get()->lora;

    tiny_snprintf(status_, sizeof(status_), "j %" PRIu32 " txs %" PRIu32 " txf %" PRIu32,
                  lora.join_failures, lora.tx_successes, lora.tx_failures);

    if (!lora.has_module) {
        display->simple({ "No Module!", status_ });
        return;
    }

    if (lora.joined > 0) {
        display->simple({ "Sending", status_ });
    }
    else {
        display->simple({ "Joining", status_ });
    }
}

void LoraView::hide() {
    get_ipc()->signal_workers(WorkerCategory::Lora, 9);
}

}
