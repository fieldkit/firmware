#include <lwcron/lwcron.h>
#include <samd51_common.h>

#include "startup_worker.h"
#include "tasks/tasks.h"
#include "self_check.h"
#include "factory_wipe.h"
#include "storage/storage.h"
#include "storage/signed_log.h"
#include "storage/data_record.h"
#include "records.h"
#include "state_ref.h"
#include "state_manager.h"
#include "status_logging.h"
#include "utilities.h"
#include "battery_status.h"
#include "clock.h"
#include "live_tests.h"

#include "readings_worker.h"
#include "upgrade_from_sd_worker.h"

#include "modules/bridge/modules.h"
#include "modules/scanning.h"
#include "modules/configure.h"
#include "hal/metal/metal_lora.h"
#include "display/display_views.h"
#include "secrets.h"
#include "graceful_shutdown.h"
#include "factory_wipe_worker.h"

extern const struct fkb_header_t fkb_header;

namespace fk {

FK_DECLARE_LOGGER("startup");

static void copy_cron_spec_from_pb(const char *name, Schedule &cs, fk_data_JobSchedule const &pb, Pool &pool);

StartupWorker::StartupWorker(bool allow_phylum) : allow_phylum_(allow_phylum) {
}

fkb_header_t const *StartupWorker::fkb_header() {
    if (fkb_header_ == nullptr) {
        return &::fkb_header;
    }
    return fkb_header_;
}

void StartupWorker::fkb_header(fkb_header_t *fkb_header) {
    fkb_header_ = fkb_header;
}

void StartupWorker::run(Pool &pool) {
    get_board()->i2c_core().begin();

#if defined(__SAMD51__)
    auto clock = get_clock();
    if (!clock->begin()) {
        logerror("rtc error");
    }

    fk_live_tests();
#endif

    loginfo("ready display");
    auto display = get_display();

    loginfo("check for self test startup");
    if (check_for_self_test_startup(pool)) {
        fk_debug_mode_configure("Self Test");
        return;
    }

    loginfo("check for provision startup");
    if (check_for_provision_startup(pool)) {
        fk_debug_mode_configure("Provision");
        return;
    }

    loginfo("check for upgrading startup");
    if (check_for_upgrading_startup(pool)) {
        fk_debug_mode_configure("Upgrading");
        return;
    }

    loginfo("check for interactive startup");
    if (check_for_interactive_startup(pool)) {
        FK_ASSERT(os_task_start(&display_task) == OSS_SUCCESS);
        return;
    }

    // Ensure we initialize the battery gauge before refreshing, since
    // we're booting. BatteryChecker assumes the gauge is ready to go.
    loginfo("check for low power startup");
    BatteryChecker battery_checker;
    battery_checker.refresh(true);

    if (!battery_checker.low_power()) {
        display->company_logo();
    }

    // NOTE Power cycle modules, this gives us a fresh start. Some times behave
    // funny, specifically temperature. Without this the first attempt down
    // below during the scan fails fails.
    // I tried moving the enable all to after the storage read and ran into the
    // same issue. After the self check seems ok, though?
    auto mm = get_modmux();
    mm->disable_all_modules();
    // Lock, just during startup.
    auto lock = mm->lock();

    // Save startup diagnostics before self-check, just incase there
    // was something wrong last time. We don't free them here, we free
    // them further on as these are for dire circumstances.
    save_captured_logs(false);

    fk_log_diagnostics();

    GlobalStateManager gsm;
    gsm.initialize(pool);

    auto memory = MemoryFactory::get_data_memory();
    if (memory->begin()) {
        if (!load_or_create_state(pool)) {
            logerror("load or create state");
        }
    }

    // Now write saved logs and free them. These will end up in the
    // named folder.
    save_captured_logs(true);

    ModuleRegistry registry;
    registry.initialize();

    if (!battery_checker.low_power()) {
        // Run self-check.
        NoopSelfCheckCallbacks noop_callbacks;
        SelfCheck self_check(display, get_network(), mm, get_module_leds());
        self_check.check(SelfCheckSettings::defaults(), noop_callbacks, &pool);

        mm->enable_all_modules();

        ReadingsWorker readings_worker{ true, true };
        readings_worker.run(pool);
    }

    // TODO Move this.
    check_for_lora(pool);

    loginfo("started");

#if defined(__SAMD51__)
    FK_ASSERT(os_task_start(&scheduler_task) == OSS_SUCCESS);
#endif
}

bool StartupWorker::load_or_create_state(Pool &pool) {
    loginfo("loading state");

    auto gs = get_global_state_rw();

    Storage storage{ MemoryFactory::get_data_memory(), pool, false, allow_phylum_ };
    if (!load_state(storage, gs.get(), pool)) {
        logwarn("problem loading state, starting fresh");

        if (!create_new_state(storage, gs.get(), pool)) {
            logerror("error creating new state");
        }

        if (!storage.flush()) {
            logerror("flush storage");
        }
    }

    get_sd_card()->name(gs.get()->general.name);

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

    MetaRecord meta_record{ pool };
    if (!storage.meta_ops()->read_record(SignedRecordKind::State, meta_record, pool)) {
        meta_record.include_state(gs, fkb_header(), pool);

        if (!storage.meta_ops()->write_record(SignedRecordKind::State, &meta_record.record(), pool)) {
            logerror("writing state");
            return false;
        }

        if (!storage.flush()) {
            logerror("flush storage");
        }

        return true;
    }

    auto &record = meta_record.record();

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

    copy_cron_spec_from_pb("readings", gs->scheduler.readings, record.schedule.readings, pool);
    copy_cron_spec_from_pb("network", gs->scheduler.network, record.schedule.network, pool);
    copy_cron_spec_from_pb("gps", gs->scheduler.gps, record.schedule.gps, pool);
    copy_cron_spec_from_pb("lora", gs->scheduler.lora, record.schedule.lora, pool);

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
        logerror("clearing storage");
        fk_logs_flush();
        fk_restart();
    }

    MetaRecord meta_record{ pool };
    meta_record.include_state(gs, fkb_header(), pool);

    if (!storage.meta_ops()->write_record(SignedRecordKind::State, &meta_record.record(), pool)) {
        logerror("writing state");
        fk_logs_flush();
        fk_restart();
    }

    return true;
}

bool StartupWorker::load_from_files(Storage &storage, GlobalState *gs, Pool &pool) {
    auto meta_ops = storage.meta_ops();
    auto meta_attributes = meta_ops->attributes(pool);
    if (!meta_attributes) {
        logerror("meta attributes");
        return false;
    }

    auto data_ops = storage.data_ops();
    auto data_attributes = data_ops->attributes(pool);
    if (!data_attributes) {
        logerror("data attributes");
        return false;
    }

    auto storage_update = StorageUpdate{
        .meta = StorageStreamUpdate{ meta_attributes->size, meta_attributes->records },
        .data = StorageStreamUpdate{ data_attributes->size, data_attributes->records },
        .nreadings = data_attributes->nreadings,
        .installed = storage.installed(),
        .used = storage.used(),
    };

    gs->apply(storage_update);

    // TODO This should be managed better.
    if (meta_attributes->records >= 2) {
        gs->transmission.meta_cursor = meta_attributes->records - 2;
    }
    else {
        gs->transmission.meta_cursor = 0;
    }
    gs->transmission.data_cursor = data_attributes->records;

    if (!load_previous_location(gs, data_ops, pool)) {
        return false;
    }

    return true;
}

bool StartupWorker::load_previous_location(GlobalState *gs, DataOps *ops, Pool &pool) {
    DataRecord record{ pool };
    if (ops->read_fixed_record(record, pool)) {
        auto &l = record.record().readings.location;
        gs->gps.latitude = l.latitude;
        gs->gps.longitude = l.longitude;
        gs->gps.altitude = l.altitude;
        gs->gps.time = l.time;
        gs->gps.satellites = l.satellites;
        gs->gps.hdop = l.hdop;
        gs->gps.fix = false;

        loginfo("(loaded) location(%f, %f)", l.longitude, l.latitude);
    }
    else {
        logwarn("unable to read saved location");
    }

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

bool StartupWorker::save_captured_logs(bool free) {
    loginfo("saving captured logs reset-reason=%s bank=%d", fk_get_reset_reason_string(), fk_nvm_get_active_bank());
    fk_logs_saved_write(!free);
    if (free) {
        fk_logs_saved_free();
    }
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

bool StartupWorker::check_for_upgrading_startup(Pool &pool) {
    auto lock = sd_mutex.acquire(UINT32_MAX);
    auto sd = get_sd_card();

    if (!sd->begin()) {
        logerror("error opening sd card");
        return false;
    }

    auto config_file = "upgrade.cfg";
    if (!sd->is_file(config_file)) {
        loginfo("no %s found", config_file);
        return false;
    }

    if (!sd->unlink(config_file)) {
        loginfo("error unlinking %s", config_file);
        return false;
    }

    GlobalStateManager gsm;
    gsm.notify("upgrading");

    FK_ASSERT(os_task_start(&display_task) == OSS_SUCCESS);

    auto swap = true;
    auto main_binary = "fk-bundled-fkb-network.bin";
    auto bl_binary = "fkbl-fkb-network.bin";
    auto params = SdCardFirmware{ SdCardFirmwareOperation::Load, bl_binary, main_binary, swap, true, OneSecondMs };
    UpgradeFirmwareFromSdWorker upgrade_worker{ params };
    upgrade_worker.run(pool);

    fk_graceful_shutdown();

    fk_logs_flush();

    fk_restart();

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

    FactoryWipeWorker factory_wipe_worker{ false };
    factory_wipe_worker.run(pool);

    fk_logs_flush();

    return true;
}

bool StartupWorker::check_for_interactive_startup(Pool &pool) {
    auto buttons = get_buttons();
    if (!buttons->is_debug_pressed()) {
        return false;
    }

    auto display = get_display();
    display->simple(SimpleScreen{ "Hold for Debug" });

    auto started = fk_uptime();
    auto enable_debug_mode = false;
    while (buttons->is_debug_pressed()) {
        fk_delay(100);

        if (fk_uptime() - started > InteractiveStartupButtonDuration) {
            display->simple(SimpleScreen{ "Release for Debug" });
            enable_debug_mode = true;
        }
    }

    if (enable_debug_mode) {
        fk_debug_mode_configure("Debug Mode");
    }

    return enable_debug_mode;
}

static void copy_cron_spec_from_pb(const char *name, Schedule &cs, fk_data_JobSchedule const &pb, Pool &pool) {
    #if defined(FK_DEBUG_OVERRIDE_SCHEDULES)
    logwarn("(ignoring) %s schedule (FK_DEBUG_OVERRIDE_SCHEDULES)", name);
    return;
    #endif

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
