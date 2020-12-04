#include <lwcron/lwcron.h>
#include <samd51_common.h>

#include "startup_worker.h"
#include "tasks/tasks.h"
#include "self_check.h"
#include "factory_wipe.h"
#include "storage/storage.h"
#include "storage/signed_log.h"
#include "storage/meta_ops.h"
#include "storage/data_record.h"
#include "records.h"
#include "state_ref.h"
#include "state_manager.h"
#include "status_logging.h"
#include "utilities.h"
#include "battery_status.h"

#include "readings_worker.h"
#include "upgrade_from_sd_worker.h"

#include "modules/bridge/modules.h"
#include "modules/scanning.h"
#include "modules/configure.h"
#include "hal/metal/metal_lora.h"
#include "display/display_views.h"
#include "secrets.h"

extern const struct fkb_header_t fkb_header;

namespace fk {

FK_DECLARE_LOGGER("startup");

static void copy_cron_spec_from_pb(const char *name, Schedule &cs, fk_data_JobSchedule &pb, Pool &pool);

void StartupWorker::run(Pool &pool) {
    loginfo("ready display");
    auto display = get_display();

    loginfo("check for self test startup");
    if (check_for_self_test_startup(pool)) {
        return;
    }

    loginfo("check for provision startup");
    if (check_for_provision_startup(pool)) {
        return;
    }

    loginfo("check for interactive startup");
    if (check_for_interactive_startup(pool)) {
        FK_ASSERT(os_task_start(&display_task) == OSS_SUCCESS);
        return;
    }

    display->company_logo();

    loginfo("ready hardware");
    auto mm = get_modmux();

    // NOTE Power cycle modules, this gives us a fresh start. Some times behave
    // funny, specifically temperature. Without this the first attempt down
    // below during the scan fails fails.
    // I tried moving the enable all to after the storage read and ran into the
    // same issue. After the self check seems ok, though?
    mm->disable_all_modules();
    // Lock, just during startup.
    auto lock = mm->lock();

    // Save startup diagnostics before self-check, just incase there
    // was something wrong last time.
    save_startup_diagnostics();

    // Run self-check.
    NoopSelfCheckCallbacks noop_callbacks;
    SelfCheck self_check(display, get_network(), mm, get_module_leds());
    self_check.check(SelfCheckSettings::defaults(), noop_callbacks, &pool);

    loginfo("prime global state");

    FK_ASSERT(fk_log_diagnostics());

    GlobalStateManager gsm;
    FK_ASSERT(gsm.initialize(pool));

    FK_ASSERT(check_for_lora(pool));

    loginfo("loading/creating state");

    FK_ASSERT(load_or_create_state(pool));

    BatteryStatus battery;
    battery.refresh();

    ModuleRegistry registry;
    registry.initialize();

    mm->enable_all_modules();

    ReadingsWorker readings_worker{ true, true, true };
    readings_worker.run(pool);

    FK_ASSERT(os_task_start(&scheduler_task) == OSS_SUCCESS);
}

bool StartupWorker::load_or_create_state(Pool &pool) {
    auto gs = get_global_state_rw();

    Storage storage{ MemoryFactory::get_data_memory(), pool, false };
    if (!load_state(storage, gs.get(), pool)) {
        logwarn("problem loading state, starting fresh");

        if (!create_new_state(storage, gs.get(), pool)) {
            logerror("error creating new state");
        }
    }

    // The preconfigured LoRa ABP state always takes precedence.
    // TODO Hack
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

bool StartupWorker::load_state(Storage &storage, GlobalState *gs, Pool &pool) {
    if (!storage.begin()) {
        return false;
    }

    if (!load_from_files(storage, gs, pool)) {
        return false;
    }

    auto meta = storage.file(Storage::Meta);
    auto srl = SignedRecordLog{ meta };
    if (!srl.seek_record(SignedRecordKind::State)) {
        return true;
    }

    auto record = fk_data_record_decoding_new(pool);
    record.identity.name.arg = reinterpret_cast<void *>(&pool);
    record.metadata.generation.arg = reinterpret_cast<void *>(&pool);
    if (!srl.decode(&record, fk_data_DataRecord_fields, pool)) {
        return true;
    }

    auto name = reinterpret_cast<const char *>(record.identity.name.arg);
    strncpy(gs->general.name, name, sizeof(gs->general.name));

    auto generation = reinterpret_cast<pb_data_t *>(record.metadata.generation.arg);
    FK_ASSERT(generation->length == GenerationLength);
    memcpy(gs->general.generation, generation->buffer, GenerationLength);

    char gen_string[GenerationLength * 2 + 1];
    bytes_to_hex_string(gen_string, sizeof(gen_string), gs->general.generation, GenerationLength);

    loginfo("(loaded) name: '%s'", gs->general.name);
    loginfo("(loaded) generation: %s", gen_string);

    auto app_eui = pb_get_data_if_provided(record.lora.appEui.arg, pool);
    if (app_eui != nullptr) {
        FK_ASSERT(app_eui->length == LoraAppEuiLength);
        FK_ASSERT(app_eui->length == sizeof(gs->lora.app_eui));
        memcpy(gs->lora.app_eui, app_eui->buffer, app_eui->length);
        loginfo("(loaded) lora app eui: %s", pb_data_to_hex_string(app_eui, pool));
    }

    auto app_key = pb_get_data_if_provided(record.lora.appKey.arg, pool);
    if (app_key != nullptr) {
        FK_ASSERT(app_key->length == LoraAppKeyLength);
        FK_ASSERT(app_key->length == sizeof(gs->lora.app_key));
        memcpy(gs->lora.app_key, app_key->buffer, app_key->length);
        loginfo("(loaded) lora app key: %s", pb_data_to_hex_string(app_key, pool));
    }

    auto app_session_key = pb_get_data_if_provided(record.lora.appSessionKey.arg, pool);
    if (app_session_key != nullptr) {
        FK_ASSERT(app_session_key->length == LoraAppSessionKeyLength);
        FK_ASSERT(app_session_key->length == sizeof(gs->lora.app_session_key));
        memcpy(gs->lora.app_session_key, app_session_key->buffer, app_session_key->length);
        loginfo("(loaded) lora app session key: %s", pb_data_to_hex_string(app_session_key, pool));
    }

    auto network_session_key = pb_get_data_if_provided(record.lora.networkSessionKey.arg, pool);
    if (network_session_key != nullptr) {
        FK_ASSERT(network_session_key->length == LoraNetworkSessionKeyLength);
        FK_ASSERT(network_session_key->length == sizeof(gs->lora.network_session_key));
        memcpy(gs->lora.network_session_key, network_session_key->buffer, network_session_key->length);
        loginfo("(loaded) lora network session key: %s", pb_data_to_hex_string(network_session_key, pool));
    }

    auto device_address = pb_get_data_if_provided(record.lora.deviceAddress.arg, pool);
    if (device_address != nullptr) {
        FK_ASSERT(device_address->length == LoraDeviceAddressLength);
        FK_ASSERT(device_address->length == sizeof(gs->lora.device_address));
        memcpy(gs->lora.device_address, device_address->buffer, device_address->length);
        loginfo("(loaded) lora device address: %s", pb_data_to_hex_string(device_address, pool));
    }

    gs->lora.uplink_counter = record.lora.uplinkCounter;
    gs->lora.downlink_counter = record.lora.downlinkCounter;

    if (app_key != nullptr) {
        gs->lora.configured = true;
    }

    if (app_session_key != nullptr && network_session_key != nullptr && device_address != nullptr) {
        gs->lora.configured = true;
    }

    auto networks_array = reinterpret_cast<pb_array_t *>(record.network.networks.arg);
    if (networks_array->length > 0) {
        FK_ASSERT(networks_array->length <= WifiMaximumNumberOfNetworks);

        auto networks = reinterpret_cast<fk_app_NetworkInfo *>(networks_array->buffer);
        for (auto i = 0u; i < networks_array->length; ++i) {
            auto &n = networks[i];
            auto ssid = reinterpret_cast<const char *>(n.ssid.arg);
            auto password = reinterpret_cast<const char *>(n.password.arg);

            if (strlen(ssid) > 0) {
                loginfo("(loaded) [%d] network: %s", i, ssid);

                auto &nc = gs->network.config.wifi_networks[i];
                strncpy(nc.ssid, ssid, sizeof(nc.ssid));
                strncpy(nc.password, password, sizeof(nc.password));
                nc.valid = nc.ssid[0] != 0;
                nc.create = false;
            }
        }
    }

    if (record.condition.recording > 0) {
        gs->general.recording = record.condition.recording;
        loginfo("(loaded) recording (%" PRIu32 ")", record.condition.recording);
    }

    if (!fk_debug_override_schedules()) {
        copy_cron_spec_from_pb("readings", gs->scheduler.readings, record.schedule.readings, pool);
        copy_cron_spec_from_pb("network", gs->scheduler.network, record.schedule.network, pool);
        copy_cron_spec_from_pb("gps", gs->scheduler.gps, record.schedule.gps, pool);
        copy_cron_spec_from_pb("lora", gs->scheduler.lora, record.schedule.lora, pool);
    }
    else {
        logwarn("ignored loaded schedules, debugger attached");
    }

    // Check for a need to fixup the duration.
    if (gs->scheduler.network.duration == 0) {
        gs->scheduler.network.duration = FiveMinutesSeconds;
        logwarn("using five minute network duration (zero)");
    }
    if (gs->scheduler.network.duration == FiveMinutesMs) {
        gs->scheduler.network.duration = FiveMinutesSeconds;
        logwarn("using five minute network duration (from ms value)");
    }
    if (gs->scheduler.gps.duration == 0) {
        gs->scheduler.gps.duration = TenMinutesSeconds;
        logwarn("using ten minute gps duration (zero)");
    }

    auto url = pb_get_string_if_provided(record.transmission.wifi.url.arg, pool);
    auto token = pb_get_string_if_provided(record.transmission.wifi.token.arg, pool);
    if (url != nullptr) {
        strncpy(gs->transmission.url, url, sizeof(gs->transmission.url));
        loginfo("(loaded) transmission url: %s", gs->transmission.url);
    }
    if (token != nullptr) {
        strncpy(gs->transmission.token, token, sizeof(gs->transmission.token));
        loginfo("(loaded) transmission token: %s", gs->transmission.token);
    }

    return true;
}

bool StartupWorker::create_new_state(Storage &storage, GlobalState *gs, Pool &pool) {
    if (!storage.clear()) {
        logerror("error clearing storage");
        fk_logs_flush();
        fk_restart();
    }

    MetaOps ops{ storage };

    if (!ops.write_state(gs, &fkb_header, pool)) {
        logerror("error writing state");
        fk_logs_flush();
        fk_restart();
    }

    return true;
}

bool StartupWorker::load_from_files(Storage &storage, GlobalState *gs, Pool &pool) {
    auto meta_fh = storage.file_header(Storage::Meta);
    auto data_fh = storage.file_header(Storage::Data);

    /**
     * We intentionally use a known earlier starting point here, the
     * positions in the block header so we can overlap the recording
     * since we aren't keeping fine grained track yet of what's
     * uploaded.
     */
    gs->transmission.data_cursor = data_fh.record;
    gs->transmission.meta_cursor = meta_fh.record - 1;

    /**
     * Now actually do seeks to the end to find the accurate sizes and
     * positions of each file.
     */
    {
        auto meta = storage.file(Storage::Meta);
        if (!meta.seek_end()) {
            return false;
        }

        gs->update_meta_stream(meta);

        loginfo("read file state (meta) R-%" PRIu32, gs->storage.meta.block);
    }

    {
        auto data = storage.file(Storage::Data);
        if (!data.seek_end()) {
            return false;
        }

        gs->update_data_stream(data);

        loginfo("read file state (data) R-%" PRIu32, gs->storage.data.block);

        // This function depends on data being seeked to the end.
        if (!load_previous_location(storage, gs, data, pool)) {
            return false;
        }
    }

    return true;
}

bool StartupWorker::load_previous_location(Storage &storage, GlobalState *gs, File &data, Pool &pool) {
    // How do we verify we're seeked to the end? NOTE
    if (!data.rewind()) {
        return false;
    }

    DataRecord record;
    auto nread = data.read(&record.for_decoding(pool), fk_data_DataRecord_fields);
    if (nread <= 0) {
        return false;
    }

    auto &l = record.record().readings.location;
    gs->gps.latitude = l.latitude;
    gs->gps.longitude = l.longitude;
    gs->gps.altitude = l.altitude;
    gs->gps.time = l.time;
    gs->gps.satellites = l.satellites;
    gs->gps.hdop = l.hdop;
    gs->gps.fix = false;

    loginfo("(loaded) location(%f, %f)", l.longitude, l.latitude);

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

bool StartupWorker::save_startup_diagnostics() {
    loginfo("saving startup diagnostics (%s) (bank = %d)", fk_get_reset_reason_string(), fk_nvm_get_active_bank());
    fk_logs_write_saved_and_free();
    return true;
}

bool StartupWorker::check_for_self_test_startup(Pool &pool) {
    auto lock = sd_mutex.acquire(UINT32_MAX);
    auto sd = get_sd_card();

    if (!sd->begin()) {
        logerror("error opening sd card");
        return false;
    }

    auto config_file = "testing.cfg";
    if (!sd->is_file(config_file)) {
        loginfo("no %s found", config_file);
        return false;
    }

    GlobalStateManager gsm;
    gsm.notify("self test");

    FK_ASSERT(os_task_start(&display_task) == OSS_SUCCESS);

    // This is so risky, thankfully we're rarely in here. This is here
    // to allow the display task to start so the controller is available.
    fk_delay(100);

    auto vc = ViewController::get();
    if (vc != nullptr) {
        vc->show_self_check();
    }

    return true;
}

bool StartupWorker::check_for_provision_startup(Pool &pool) {
    auto lock = sd_mutex.acquire(UINT32_MAX);
    auto sd = get_sd_card();

    if (!sd->begin()) {
        logerror("error opening sd card");
        return false;
    }

    auto config_file = "fk.cfg";
    if (!sd->is_file(config_file)) {
        loginfo("no %s found", config_file);
        return false;
    }

    GlobalStateManager gsm;
    gsm.notify("provisioning");

    FK_ASSERT(os_task_start(&display_task) == OSS_SUCCESS);

    auto swap = true;
    auto main_binary = "fk-bundled-fkb.bin";
    auto bl_binary = "fkbl-fkb.bin";
    auto params = SdCardFirmware{ SdCardFirmwareOperation::Load, bl_binary, main_binary, swap, true, OneSecondMs };
    UpgradeFirmwareFromSdWorker upgrade_worker{ params };
    upgrade_worker.run(pool);

    fk_logs_flush();

    return true;
}

bool StartupWorker::check_for_interactive_startup(Pool &pool) {
    auto buttons = get_buttons();
    if (buttons->number_pressed() == 0) {
        return false;
    }

    auto display = get_display();
    display->simple(SimpleScreen{ "Hold for Debug" });

    auto started = fk_uptime();
    auto enable_debug_mode = false;
    while (buttons->number_pressed() > 0) {
        fk_delay(100);

        if (fk_uptime() - started > InteractiveStartupButtonDuration) {
            display->simple(SimpleScreen{ "Release for Debug" });
            enable_debug_mode = true;
        }
    }

    return enable_debug_mode;
}

static void copy_cron_spec_from_pb(const char *name, Schedule &cs, fk_data_JobSchedule &pb, Pool &pool) {
    auto pbd = pb_get_data_if_provided(pb.cron.arg, pool);
    if (pbd != nullptr) {
        FK_ASSERT(pbd->length == sizeof(lwcron::CronSpec));
        memcpy(&cs.cron, pbd->buffer, pbd->length);
    }

    auto intervals_array = reinterpret_cast<pb_array_t *>(pb.intervals.arg);
    if (intervals_array != nullptr && intervals_array->length > 0) {
        auto intervals_source = reinterpret_cast<fk_app_Interval*>(intervals_array->buffer);
        for (auto i = 0u; i < std::min(intervals_array->length, MaximumScheduleIntervals); ++i) {
            cs.intervals[i].start = intervals_source[i].start;
            cs.intervals[i].end = intervals_source[i].end;
            cs.intervals[i].interval = intervals_source[i].interval;
        }
    }

    cs.interval = pb.interval;
    cs.repeated = pb.repeated;
    cs.duration = pb.duration;

    loginfo("(loaded) %s interval = %" PRIu32 " repeated = %" PRIu32 " duration = %" PRIu32, name, cs.interval, cs.repeated, cs.duration);
}

}
