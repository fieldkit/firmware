#include <os.h>

#include "lora_ranging_worker.h"

#include "hal/lora.h"
#include "lora_manager.h"
#include "platform.h"
#include "records.h"
#include "tasks/tasks.h"
#include "utilities.h"

namespace fk {

FK_DECLARE_LOGGER("lora");

LoraRangingWorker::LoraRangingWorker(bool confirmed) : confirmed_(confirmed) {
}

void LoraRangingWorker::run(Pool &pool) {
    LoraManager lora{ get_lora_network() };

    if (!lora.begin()) {
        return;
    }

    while (true) {
        StandardPool work_pool{ "lora-range" };

        logdebug("joining if necessary...");

        if (lora.join_if_necessary(work_pool)) {
            loginfo("joined!");

            fk_serial_number_t sn;
            pb_data_t device_id = {
                .length = sizeof(sn),
                .buffer = &sn,
            };

            fk_data_LoraRecord record;
            fk_lora_record_encoding_new(&record);
            record.deviceId.arg = (void *)&device_id;

            auto encoded = work_pool.encode(fk_data_LoraRecord_fields, &record);
            if (encoded == nullptr) {
                logerror("encode failed");
            } else {
                lora.send_bytes(LoraStatusPort, encoded->buffer, encoded->size, confirmed_, work_pool);
            }
        }

        loginfo("sleeping");

        fk_delay(5000);

        if (fk_task_stop_requested(nullptr)) {
            break;
        }
    }

    lora.stop();
}

} // namespace fk
