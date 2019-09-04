#include "startup_worker.h"
#include "tasks/tasks.h"
#include "self_check.h"
#include "factory_wipe.h"
#include "storage/storage.h"
#include "storage/signed_log.h"
#include "readings_worker.h"
#include "records.h"
#include "state_ref.h"

namespace fk {

FK_DECLARE_LOGGER("sw");

void StartupWorker::run(Pool &pool) {
    NoopSelfCheckCallbacks noop_callbacks;
    SelfCheck self_check(get_display(), get_network(), get_modmux());
    self_check.check(SelfCheckSettings{ }, noop_callbacks);

    Storage storage{ MemoryFactory::get_data_memory() };
    if (storage.begin()) {
        auto meta = storage.file(Storage::Meta);
        auto srl = SignedRecordLog{ meta };
        if (srl.seek_record(SignedRecordKind::State)) {
            auto record = fk_data_record_decoding_new(pool);
            record.identity.name.arg = (void *)&pool;
            if (srl.decode(&record, fk_data_DataRecord_fields, pool)) {
                auto gs = get_global_state_rw();
                auto name = (const char *)record.identity.name.arg;
                strncpy(gs.get()->general.name, name, sizeof(gs.get()->general.name));
                gs.get()->general.recording = (record.condition.flags & fk_data_ConditionFlags_CONDITION_FLAGS_RECORDING) > 0;
                loginfo("found custom name '%s'", name);
            }
        }
    }

    FactoryWipe fw{ get_buttons(), &storage };
    FK_ASSERT(fw.wipe_if_necessary());

    ReadingsWorker readings_worker;
    readings_worker.run(pool);

    FK_ASSERT(os_task_start(&scheduler_task) == OSS_SUCCESS);
}

}
