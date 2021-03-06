#include "state_manager.h"
#include "state.h"
#include "device_name.h"
#include "state_ref.h"
#include "hal/random.h"
#include "utilities.h"
#include "secrets.h"
#include "clock.h"

namespace fk {

FK_DECLARE_LOGGER("gsm");

static void initialize_compile_time_wifi(WifiNetworkInfo &network, const char *ssid, const char *password) __attribute__ ((unused));

static void initialize_compile_time_wifi(WifiNetworkInfo &network, const char *ssid, const char *password) {
    loginfo("(hardcoded) wifi '%s'", ssid);
    strncpy(network.ssid, ssid, sizeof(network.ssid));
    strncpy(network.password, password, sizeof(network.password));
    network.valid = true;
}

bool GlobalStateManager::initialize(Pool &pool) {
    auto gs = get_global_state_rw();

    gs.get()->runtime.startup_time = get_clock_now();

    auto name = fk_device_name_generate(pool);

    strncpy(gs.get()->general.name, name, sizeof(gs.get()->general.name));

    FK_ASSERT(fk_random_fill_u8(gs.get()->general.generation, sizeof(gs.get()->general.generation)) == GenerationLength);

    char gen_string[GenerationLength * 2 + 1];
    bytes_to_hex_string(gen_string, sizeof(gen_string), gs.get()->general.generation, sizeof(gs.get()->general.generation));
    loginfo("(generated) name: '%s'", gs.get()->general.name);
    loginfo("(generated) gen: %s", gen_string);

    for (auto i = 0u; i < WifiMaximumNumberOfNetworks; ++i) {
        auto &nc = gs.get()->network.config.wifi_networks[i];
        nc.valid = false;
        nc.create = false;
        nc.ssid[0] = 0;
        nc.password[0] = 0;
    }

    #if defined(FK_WIFI_0_SSID) && defined(FK_WIFI_0_PASSWORD)
    initialize_compile_time_wifi(gs.get()->network.config.wifi_networks[0], FK_WIFI_0_SSID, FK_WIFI_0_PASSWORD);
    #endif

    #if defined(FK_WIFI_1_SSID) && defined(FK_WIFI_1_PASSWORD)
    initialize_compile_time_wifi(gs.get()->network.config.wifi_networks[1], FK_WIFI_1_SSID, FK_WIFI_1_PASSWORD);
    #endif

    #if defined(FK_LORA_APP_KEY)
    loginfo("(hardcoded) lora app key '%s'", FK_LORA_APP_KEY);
    hex_string_to_bytes(gs.get()->lora.app_key, sizeof(gs.get()->lora.app_key), FK_LORA_APP_KEY);
    gs.get()->lora.configured = true;
    #endif

    #if defined(FK_LORA_APP_EUI)
    loginfo("(hardcoded) lora app eui '%s'", FK_LORA_APP_EUI);
    hex_string_to_bytes(gs.get()->lora.app_eui, sizeof(gs.get()->lora.app_eui), FK_LORA_APP_EUI);
    #else
    bzero(gs.get()->lora.app_eui, sizeof(gs.get()->lora.app_eui));
    #endif

    if (fk_debug_get_console_attached()) {
        gs.get()->scheduler.readings.interval = DefaultDebugReadingsInterval;
        gs.get()->scheduler.network.interval = DefaultDebugNetworkInterval;
        gs.get()->scheduler.gps.interval = DefaultDebugGpsInterval;
        gs.get()->scheduler.lora.interval = DefaultDebugLoraInterval;
        gs.get()->scheduler.network.duration = FiveMinutesSeconds;
        loginfo("using debug schedule");
    }
    else {
        gs.get()->scheduler.readings.interval = DefaultReadingsInterval;
        gs.get()->scheduler.network.interval = DefaultNetworkInterval;
        gs.get()->scheduler.gps.interval = DefaultGpsInterval;
        gs.get()->scheduler.lora.interval = DefaultLoraInterval;
        gs.get()->scheduler.network.duration = FiveMinutesSeconds;
        loginfo("using default schedule");
    }

    gs.get()->scheduler.readings.recreate();
    gs.get()->scheduler.network.recreate();
    gs.get()->scheduler.gps.recreate();
    gs.get()->scheduler.lora.recreate();

    gs.get()->transmission.url[0] = 0;
    gs.get()->transmission.token[0] = 0;

    return true;
}

bool GlobalStateManager::rebuild() {
    return true;
}

}
