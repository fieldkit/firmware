#include "startup_worker.h"
#include "tasks/tasks.h"
#include "self_check.h"
#include "factory_wipe.h"
#include "storage/storage.h"
#include "storage/signed_log.h"
#include "storage/meta_ops.h"
#include "readings_worker.h"
#include "records.h"
#include "state_ref.h"
#include "state_manager.h"
#include "utilities.h"

#include "modules/bridge/modules.h"
#include "modules/scanning.h"
#include "modules/configure.h"

#include "hal/metal/metal_lora.h"

namespace fk {

FK_DECLARE_LOGGER("startup");

void StartupWorker::run(Pool &pool) {
    auto display = get_display();

    display->company_logo();

    GlobalStateManager gsm;
    FK_ASSERT(gsm.initialize(pool));

    if (check_for_interactive_startup()) {
        FK_ASSERT(os_task_start(&display_task) == OSS_SUCCESS);
        return;
    }

    FK_ASSERT(check_for_lora(pool));

    auto mm = get_modmux();

    // NOTE Power cycle modules, this gives us a fresh start. Some times behave
    // funny, specifically temperature. Without this the first attempt down
    // below during the scan fails fails.
    // I tried moving the enable all to after the storage read and ran into the
    // same issue. After the self check seems ok, though?
    mm->disable_all_modules();

    // Lock, just during startup.
    auto lock = get_board()->lock_eeprom();

    NoopSelfCheckCallbacks noop_callbacks;
    SelfCheck self_check(display, get_network(), mm);
    self_check.check(SelfCheckSettings{ }, noop_callbacks);

    // TODO Only do this if the last fsck was a while ago?
    Storage storage{ MemoryFactory::get_data_memory(), false };
    if (storage.begin()) {
        NoopProgressCallbacks progress;
        storage.fsck(&progress);
    }

    FK_ASSERT(load_or_create_state(storage, pool));

    mm->enable_all_modules();

    ReadingsWorker readings_worker{ true };
    readings_worker.run(pool);

    FK_ASSERT(os_task_start(&scheduler_task) == OSS_SUCCESS);
}

bool StartupWorker::check_for_interactive_startup() {
    auto buttons = get_buttons();

    if (buttons->number_pressed() == 0) {
        return false;
    }

    auto display = get_display();

    display->simple({ "Hold for Debug" });

    auto started = fk_uptime();
    auto enable_debug_mode = false;
    while (buttons->number_pressed() > 0) {
        fk_delay(100);

        if (fk_uptime() - started > InteractiveStartupButtonDuration) {
            display->simple({ "Release for Debug" });
            enable_debug_mode = true;
        }
    }

    return enable_debug_mode;
}

bool StartupWorker::load_or_create_state(Storage &storage, Pool &pool) {
    if (!load_state(storage, pool)) {
        logwarn("problem loading state, starting fresh");

        FK_ASSERT(create_new_state(storage, pool));
    }

    return true;
}

bool StartupWorker::load_state(Storage &storage, Pool &pool) {
    if (storage.begin()) {
        auto meta = storage.file(Storage::Meta);
        auto srl = SignedRecordLog{ meta };
        if (srl.seek_record(SignedRecordKind::State)) {
            auto record = fk_data_record_decoding_new(pool);
            record.identity.name.arg = (void *)&pool;
            record.metadata.generation.arg = (void *)&pool;
            if (srl.decode(&record, fk_data_DataRecord_fields, pool)) {
                auto gs = get_global_state_rw();
                auto name = (const char *)record.identity.name.arg;
                auto generation = (uint8_t *)record.metadata.generation.arg;

                strncpy(gs.get()->general.name, name, sizeof(gs.get()->general.name));
                memcpy(gs.get()->general.generation, generation, GenerationLength);

                char gen_string[GenerationLength * 2 + 1];
                bytes_to_hex_string(gen_string, sizeof(gen_string), gs.get()->general.generation, GenerationLength);

                loginfo("(loaded) name: '%s'", gs.get()->general.name);
                loginfo("(loaded) generation: %s", gen_string);

                auto app_eui = pb_get_data_if_provided(record.lora.appEui.arg, pool);
                if (app_eui != nullptr) {
                    FK_ASSERT(app_eui->length == LoraAppEuiLength);
                    FK_ASSERT(app_eui->length == sizeof(gs.get()->lora.app_eui));
                    memcpy(gs.get()->lora.app_eui, app_eui->buffer, app_eui->length);
                    loginfo("(loaded) lora app eui: %s", pb_data_to_hex_string(app_eui, pool));
                }

                auto app_key = pb_get_data_if_provided(record.lora.appKey.arg, pool);
                if (app_key != nullptr) {
                    FK_ASSERT(app_key->length == LoraAppKeyLength);
                    FK_ASSERT(app_key->length == sizeof(gs.get()->lora.app_key));
                    memcpy(gs.get()->lora.app_key, app_key->buffer, app_key->length);
                    loginfo("(loaded) lora app key: %s", pb_data_to_hex_string(app_key, pool));
                    gs.get()->lora.configured = true;
                }

                auto networks_array = (pb_array_t *)record.network.networks.arg;
                if (networks_array->length > 0) {
                    FK_ASSERT(networks_array->length <= MaximumNumberOfWifiNetworks);

                    auto networks = (fk_app_NetworkInfo *)networks_array->buffer;
                    for (auto i = 0u; i < MaximumNumberOfWifiNetworks; ++i) {
                        auto &n = networks[i];
                        auto ssid = (const char *)n.ssid.arg;
                        auto password = (const char *)n.password.arg;

                        if (strlen(ssid) > 0) {
                            loginfo("(loaded) [%d] network: %s", i, ssid);

                            auto &nc = gs.get()->network.config.wifi_networks[i];
                            strncpy(nc.ssid, ssid, sizeof(nc.ssid));
                            strncpy(nc.password, password, sizeof(nc.password));
                            nc.valid = nc.ssid[0] != 0;
                            nc.create = false;
                        }
                    }
                }

                gs.get()->general.recording = (record.condition.flags & fk_data_ConditionFlags_CONDITION_FLAGS_RECORDING) > 0;

                if (gs.get()->general.recording) {
                    loginfo("(loaded) recording");
                }

                return true;
            }
        }
    }

    return false;
}

bool StartupWorker::create_new_state(Storage &storage, Pool &pool) {
    auto gs = get_global_state_rw();

    FK_ASSERT(storage.clear());

    MetaOps ops{ storage };

    FK_ASSERT(ops.write_state(gs.get(), pool));

    return true;
}

bool StartupWorker::check_for_lora(Pool &pool) {
    auto gs = get_global_state_rw();
    auto lora = get_lora_network();

    if (!lora->begin()) {
        return true;
    }

    auto device_eui = lora->device_eui();

    memcpy(gs.get()->lora.device_eui, device_eui, LoraDeviceEuiLength);

    loginfo("(loaded) lora device eui: %s", bytes_to_hex_string_pool(device_eui, LoraDeviceEuiLength, pool));

    return true;
}

}
