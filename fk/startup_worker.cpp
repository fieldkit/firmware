#include "startup_worker.h"
#include "tasks/tasks.h"
#include "self_check.h"
#include "factory_wipe.h"
#include "storage/storage.h"
#include "storage/signed_log.h"
#include "readings_worker.h"
#include "records.h"
#include "state_ref.h"

#include "modules/bridge/modules.h"
#include "modules/scanning.h"
#include "modules/configure.h"

namespace fk {

FK_DECLARE_LOGGER("sw");

static void developer_configuration() {
    auto module_bus = get_board()->i2c_module();
    auto gs = get_global_state_ro();

    ModuleContext mc{ gs.get(), module_bus };
    Storage storage{ nullptr }; // NOTE: Not opened!
    ModuleScanning scanning{ get_modmux() };
    ModuleConfigurer configurer{ scanning };

    // FK_ASSERT(configurer.weather(4));
    // FK_ASSERT(configurer.water(0));
    // FK_ASSERT(configurer.water(2));
    // FK_ASSERT(configurer.water(4));
    // FK_ASSERT(configurer.water(6));
}

void StartupWorker::run(Pool &pool) {
    auto display = get_display();
    display->company_logo();

    auto mm = get_modmux();

    // NOTE Power cycle modules, this gives us a fresh start. Some times behave
    // funny, specifically temperature. Without this the first attempt down
    // below during the scan fails fails.
    // I tried moving the enable all to after the storage read and ran into the
    // same issue. After the self check seems ok, though?

    mm->disable_all_modules();

    NoopSelfCheckCallbacks noop_callbacks;
    SelfCheck self_check(display, get_network(), mm);
    self_check.check(SelfCheckSettings{ }, noop_callbacks);

    developer_configuration();

    mm->enable_all_modules();

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

    FactoryWipe fw{ display, get_buttons(), &storage };
    FK_ASSERT(fw.wipe_if_necessary());

    ReadingsWorker readings_worker{ true };
    readings_worker.run(pool);

    FK_ASSERT(os_task_start(&scheduler_task) == OSS_SUCCESS);
}

}
