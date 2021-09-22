#include <os.h>

#include "lora_ranging_worker.h"

#include "hal/lora.h"
#include "hal/random.h"
#include "hal/hal.h"
#include "lora_manager.h"
#include "platform.h"
#include "records.h"
#include "tasks/tasks.h"
#include "utilities.h"

namespace fk {

FK_DECLARE_LOGGER("lora");

LoraRangingWorker::LoraRangingWorker() {
}

void LoraRangingWorker::run(Pool &pool) {
    auto lock = lora_mutex.acquire(0);
    if (!lock) {
        loginfo("already running");
        return;
    }

    LoraManager lora{ get_lora_network() };
    if (!lora.begin(pool)) {
        return;
    }

    while (true) {
        loop(lora);

        auto delay = FiveSecondsMs * 4;
        loginfo("sleeping for %" PRIu32, "ms", delay);
        fk_delay(delay);
        if (fk_task_stop_requested(nullptr)) {
            break;
        }
    }

    lora.stop();
}

void LoraRangingWorker::loop(LoraManager &lora) {
    StandardPool pool{ "lora-range" };

    logdebug("joining if necessary...");

    if (lora.join_if_necessary(pool)) {
        loginfo("joined!");

        fk_serial_number_t sn;
        pb_data_t device_id = {
            .length = sizeof(sn),
            .buffer = &sn,
        };

        fk_data_LoraRecord record;
        fk_lora_record_encoding_new(&record);
        record.deviceId.arg = (void *)&device_id;

        auto encoded = pool.encode(fk_data_LoraRecord_fields, &record);
        if (encoded == nullptr) {
            logerror("encode failed");
        } else {
            lora.send_bytes(LoraStatusPort, encoded->buffer, encoded->size, pool);
        }
    }
}

} // namespace fk
