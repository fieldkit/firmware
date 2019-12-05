#include <lwcron/lwcron.h>

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
#include "secrets.h"

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

    Storage storage{ MemoryFactory::get_data_memory(), false };
    if (storage.begin()) {
        // TODO Only do this if the last fsck was a while ago?
        // storage.fsck(&noop_callbacks);
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

static void copy_cron_spec_from_pb(const char *name, Schedule &cs, fk_data_JobSchedule &pb, Pool &pool) {
    auto pbd = pb_get_data_if_provided(pb.cron.arg, pool);
    if (pbd != nullptr) {
        FK_ASSERT(pbd->length == sizeof(lwcron::CronSpec));
        memcpy(&cs.cron, pbd->buffer, pbd->length);
    }
    cs.interval = pb.interval;
    loginfo("(loaded) %s interval = %" PRIu32, name, cs.interval);
}

bool StartupWorker::load_state(Storage &storage, Pool &pool) {
    if (!storage.begin()) {
        return false;
    }

    auto meta = storage.file(Storage::Meta);
    auto srl = SignedRecordLog{ meta };
    if (!srl.seek_record(SignedRecordKind::State)) {
        return true;
    }

    auto record = fk_data_record_decoding_new(pool);
    record.identity.name.arg = (void *)&pool;
    record.metadata.generation.arg = (void *)&pool;
    if (!srl.decode(&record, fk_data_DataRecord_fields, pool)) {
        return true;
    }

    auto gs = get_global_state_rw();
    auto name = (const char *)record.identity.name.arg;
    auto generation = (pb_data_t *)record.metadata.generation.arg;

    strncpy(gs.get()->general.name, name, sizeof(gs.get()->general.name));

    if (generation->length == GenerationLength) {
        memcpy(gs.get()->general.generation, generation->buffer, GenerationLength);
    }

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
    }

    auto app_session_key = pb_get_data_if_provided(record.lora.appSessionKey.arg, pool);
    if (app_session_key != nullptr) {
        FK_ASSERT(app_session_key->length == LoraAppSessionKeyLength);
        FK_ASSERT(app_session_key->length == sizeof(gs.get()->lora.app_session_key));
        memcpy(gs.get()->lora.app_session_key, app_session_key->buffer, app_session_key->length);
        loginfo("(loaded) lora app session key: %s", pb_data_to_hex_string(app_session_key, pool));
    }

    auto network_session_key = pb_get_data_if_provided(record.lora.networkSessionKey.arg, pool);
    if (network_session_key != nullptr) {
        FK_ASSERT(network_session_key->length == LoraNetworkSessionKeyLength);
        FK_ASSERT(network_session_key->length == sizeof(gs.get()->lora.network_session_key));
        memcpy(gs.get()->lora.network_session_key, network_session_key->buffer, network_session_key->length);
        loginfo("(loaded) lora network session key: %s", pb_data_to_hex_string(network_session_key, pool));
    }

    auto device_address = pb_get_data_if_provided(record.lora.deviceAddress.arg, pool);
    if (device_address != nullptr) {
        FK_ASSERT(device_address->length == LoraDeviceAddressLength);
        FK_ASSERT(device_address->length == sizeof(gs.get()->lora.device_address));
        memcpy(gs.get()->lora.device_address, device_address->buffer, device_address->length);
        loginfo("(loaded) lora device address: %s", pb_data_to_hex_string(device_address, pool));
    }

    gs.get()->lora.uplink_counter = record.lora.uplinkCounter;
    gs.get()->lora.downlink_counter = record.lora.downlinkCounter;

    if (app_key != nullptr) {
        gs.get()->lora.configured = true;
    }

    if (app_session_key != nullptr && network_session_key != nullptr && device_address != nullptr) {
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

    if (record.condition.recording > 0) {
        gs.get()->general.recording = record.condition.recording;
        loginfo("(loaded) recording (%" PRIu32 ")", record.condition.recording);
    }

    copy_cron_spec_from_pb("readings", gs.get()->scheduler.readings, record.schedule.readings, pool);
    copy_cron_spec_from_pb("network", gs.get()->scheduler.network, record.schedule.network, pool);
    copy_cron_spec_from_pb("gps", gs.get()->scheduler.gps, record.schedule.gps, pool);
    copy_cron_spec_from_pb("lora", gs.get()->scheduler.lora, record.schedule.lora, pool);

    loginfo("(fixed) lora abp: %zd", sizeof(lora_preconfigured_abp) / sizeof(LoraAbpSettings));

    for (auto &abp : lora_preconfigured_abp) {
        if (memcmp(gs.get()->lora.device_eui, abp.device_eui, LoraDeviceEuiLength) == 0) {
            memcpy(gs.get()->lora.device_address, abp.device_address, LoraDeviceAddressLength);
            loginfo("(fixed) lora device address: %s", bytes_to_hex_string_pool(abp.device_address, LoraDeviceAddressLength, pool));

            memcpy(gs.get()->lora.network_session_key, abp.network_session_key, LoraNetworkSessionKeyLength);
            loginfo("(fixed) lora network session key: %s", bytes_to_hex_string_pool(abp.network_session_key, LoraNetworkSessionKeyLength, pool));

            memcpy(gs.get()->lora.app_session_key, abp.app_session_key, LoraAppSessionKeyLength);
            loginfo("(fixed) lora app session key: %s", bytes_to_hex_string_pool(abp.app_session_key, LoraAppSessionKeyLength, pool));
        }
    }

    return true;
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

    // Turn the radio off for now, will stay powered when we start
    // using the thing to send data.
    lora->stop();

    return true;
}

}
