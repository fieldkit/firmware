#include <os.h>

#include "lora_ranging_worker.h"

#include "hal/lora.h"
#include "lora_manager.h"
#include "platform.h"
#include "utilities.h"
#include "records.h"

namespace fk {

FK_DECLARE_LOGGER("lora");

void LoraRangingWorker::run(Pool &pool) {
    LoraManager lora{ get_lora_network() };

    while (true) {
        MallocPool work_pool{ "lora-range", DefaultWorkerPoolSize };

        logdebug("joining if necessary...");

        if (lora.join_if_necessary(work_pool)) {
            loginfo("joined!");

            fk_serial_number_t sn;
            pb_data_t device_id = {
                .length = sizeof(sn),
                .buffer = &sn,
            };

            fk_data_LoraRecord record = fk_lora_record_encoding_new();
            record.deviceId.arg = (void *)&device_id;

            auto encoded = work_pool.encode(fk_data_LoraRecord_fields, &record);
            if (encoded == nullptr) {
                logerror("encode failed");
            }
            else {
                lora.send_bytes(LoraStatusPort, encoded->buffer, encoded->size);
            }
        }

        loginfo("sleeping");

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
