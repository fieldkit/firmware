#include <os.h>

#include "lora_ranging_worker.h"

#include "hal/lora.h"
#include "lora_manager.h"
#include "platform.h"
#include "utilities.h"

namespace fk {

FK_DECLARE_LOGGER("lora");

void LoraRangingWorker::run(Pool &pool) {
    LoraManager lora{ get_lora_network() };

    while (true) {
        MallocPool work_pool{ "lora-range", DefaultWorkerPoolSize };

        if (lora.join_if_necessary(work_pool)) {
            loginfo("joined");

            if (lora.send_bytes(LoraStatusPort, nullptr, 0)) {
                //
            }
            else {
                //
            }
        }

        fk_delay(5000);

        uint32_t signal = 0;
        if (os_signal_check(&signal) == OSS_SUCCESS) {
            if (signal > 0) {
                break;
            }
        }
    }

    lora.stop();
}

} // namespace fk
