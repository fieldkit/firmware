#include <tiny_printf.h>
#include <samd51_common.h>

#include "live_tests.h"
#include "common.h"
#include "storage/storage.h"
#include "storage/signed_log.h"
#include "modules/shared/crc.h"
#include "modules/eeprom.h"
#include "hal/random.h"
#include "hal/display.h"
#include "hal/battery_gauge.h"
#include "hal/sd_card.h"
#include "hal/hal.h"
#include "state_ref.h"
#include "state_manager.h"
#include "core_dump.h"

#include "networking/network_services.h"
#include "hal/pins.h"

#include "../modules/weather/main/weather.h"

extern const struct fkb_header_t fkb_header;

namespace fk {

FK_DECLARE_LOGGER("live-tests");

static void scan_i2c_radio_bus() __attribute__((unused));

static void scan_i2c_module_bus() __attribute__((unused));

static void wifi_test() __attribute__((unused));

static void wifi_low_level_test() __attribute__((unused));

static void scan_i2c_radio_bus() {
    auto bus = get_board()->i2c_radio();

    bus.begin();

    // get_board()->enable_lora();

    fk_delay(1000);

    while (true) {
        loginfo("scanning");

        for (auto i = 0u; i < 128u; ++i) {
            auto rv = bus.write(i, nullptr, 0);
            if (I2C_CHECK(rv)) {
                loginfo("  found 0x%x", i);
            }
        }

        fk_delay(1000);
    }
}

static void scan_bus(TwoWireWrapper &bus) {
    for (auto i = 0u; i < 128u; ++i) {
        auto rv = bus.write(i, nullptr, 0);
        if (I2C_CHECK(rv)) {
            loginfo("  found 0x%x", i);
        }
    }
}

static void scan_i2c_module_bus() {
    auto mm = get_modmux();

    mm->begin();

    mm->disable_all_modules();

    fk_delay(1000);

    mm->enable_all_modules();

    fk_delay(100);

    auto bus = get_board()->i2c_module();

    while (true) {
        for (auto i : { ModulePosition::from(1), ModulePosition::from(2), ModulePosition::from(3), ModulePosition::from(4) }) {
            if (!mm->choose(i)) {
                loginfo("unable to choose %d", i.integer());
                continue;
            }

            loginfo("position: %d", i.integer());

            fk_delay(100);

            scan_bus(bus);
        }

        fk_delay(10000);
    }
}

static bool write_header_file(const char *name, ModuleHeader &header, Pool &pool) {
    auto sd = get_sd_card();

    auto file = sd->open(name, OpenFlags::Write, pool);
    if (file == nullptr || !*file) {
        return false;
    }

    if (file->write((uint8_t *)&header, sizeof(ModuleHeader)) != sizeof(ModuleHeader)) {
        return false;
    }

    file->close();

    return true;
}

static void write_headers() {
    StandardPool pool{ "write-headers " };

    auto lock = sd_mutex.acquire(UINT32_MAX);
    auto sd = get_sd_card();

    if (!sd->begin()) {
        logerror("error opening sd card");
        return;
    }

    ModuleHeader header_weather = {
        .manufacturer = FK_MODULES_MANUFACTURER,
        .kind = FK_MODULES_KIND_WEATHER,
        .version = 0x01,
        .id = { 0 },
    };

    ModuleHeader header_water_ph = {
        .manufacturer = FK_MODULES_MANUFACTURER,
        .kind = FK_MODULES_KIND_WATER_PH,
        .version = 0x01,
        .id = { 0 },
    };

    ModuleHeader header_water_ec = {
        .manufacturer = FK_MODULES_MANUFACTURER,
        .kind = FK_MODULES_KIND_WATER_EC,
        .version = 0x01,
        .id = { 0 },
    };

    ModuleHeader header_water_do = {
        .manufacturer = FK_MODULES_MANUFACTURER,
        .kind = FK_MODULES_KIND_WATER_DO,
        .version = 0x01,
        .id = { 0 },
    };

    ModuleHeader header_water_temp = {
        .manufacturer = FK_MODULES_MANUFACTURER,
        .kind = FK_MODULES_KIND_WATER_TEMP,
        .version = 0x01,
        .id = { 0 },
    };

    ModuleHeader header_water_orp = {
        .manufacturer = FK_MODULES_MANUFACTURER,
        .kind = FK_MODULES_KIND_WATER_ORP,
        .version = 0x01,
        .id = { 0 },
    };

    write_header_file("weather.hdr", header_weather, pool);
    write_header_file("w-ph.hdr", header_water_ph, pool);
    write_header_file("w-ec.hdr", header_water_ec, pool);
    write_header_file("w-do.hdr", header_water_do, pool);
    write_header_file("w-temp.hdr", header_water_temp, pool);
    write_header_file("w-orp.hdr", header_water_orp, pool);
    if (false) {
        write_header_file("fk-program.cfg", header_weather, pool);
    }
}

static void test_core_dump() {
    // fk_core_dump("livetest");
    // fk_core_dump(nullptr);

    fk_core_dump_tasks();

    while (true) {
        fk_delay(1000);
    }
}

// #define FK_NETWORK_ESP32

static void wifi_low_level_test() {
#if defined(FK_NETWORK_ESP32)
    pinMode(WIFI_ESP32_CS, OUTPUT);
    digitalWrite(WIFI_ESP32_CS, HIGH);
    SPI1.begin();

    // WiFi.setPins(WIFI_ESP32_CS, WIFI_ESP32_ACK, WIFI_ESP32_RESET, WIFI_ESP32_GPIO0, &SPI1);

    if (WiFi.status() == WL_NO_MODULE) {
        logwarn("communication with wifi module failed!");
        while (true)
            ;
    }
    String fv = WiFi.firmwareVersion();
    loginfo("firmware: %s", fv.c_str());

    WiFi.noLowPowerMode();

    int serverPort = 80;
    WiFiServer server(serverPort);

    loginfo("attempting to connect to SSID");

    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    uint8_t status = -1;
    do {
        status = WiFi.begin("Cottonwood", "asdfasdf");
        delay(100); // wait until connection is ready!
    } while (status != WL_CONNECTED);

    loginfo("connected to wifi!");

    server.begin();

    auto total_bytes = 1024 * 1024;

    // size_t size = 2048u;
    size_t size = 1024u;
    char *buffer = (char *)malloc(size);

    while (true) {
        // listen for incoming clients
        WiFiClient client = server.available();
        if (client) {
            loginfo("New client");
            // an http request ends with a blank line
            boolean currentLineIsBlank = true;
            while (client.connected()) {
                if (client.available()) {
                    char c = client.read();
                    Serial.write(c);
                    // if you've gotten to the end of the line (received a newline
                    // character) and the line is blank, the http request has ended,
                    // so you can send a reply
                    if (c == '\n' && currentLineIsBlank) {
                        // send a standard http response header
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-Type: text/html");
                        client.println("Connection: close"); // the connection will be closed after completion of the response
                        client.print("Content-Length: ");
                        client.print(total_bytes);
                        client.println("");
                        client.println();
                        auto remaining = total_bytes;
                        memset(buffer, size, 0);
                        memset(buffer, size - 1, 'A');
                        auto tries = 0u;
                        while (remaining > 0) {
                            auto wrote = client.print(buffer);
                            if (wrote == 0) {
                                logerror("oh no, 0 bytes written (%zu total, %zu remaining)", total_bytes - remaining, remaining);
                                fk_delay(500);
                                if (tries++ == 2) {
                                    break;
                                }
                            } else {
                                tries = 0;
                            }
                            fk_delay(10);
                            remaining -= wrote;
                        }
                        break;
                    }
                    if (c == '\n') {
                        // you're starting a new line
                        currentLineIsBlank = true;
                    } else if (c != '\r') {
                        // you've gotten a character on the current line
                        currentLineIsBlank = false;
                    }
                }
            }
            // give the web browser time to receive the data
            fk_delay(1);

            // close the connection:
            client.stop();
            loginfo("disconnected");
        }

        fk_delay(10);
    }
#endif
}

static void wifi_test() {
    StandardPool pool{ "network-task" };
    NetworkServices services{ get_network(), pool };
    GlobalStateManager gsm;

    gsm.apply([=](GlobalState *gs) {
        gs->network.config.wifi_networks[0] = {};
        gs->network.config.wifi_networks[0].valid = true;
        strncpy(gs->network.config.wifi_networks[0].ssid, "Cottonwood", sizeof(gs->network.config.wifi_networks[0].ssid));
        strncpy(gs->network.config.wifi_networks[0].password, "asdfasdf", sizeof(gs->network.config.wifi_networks[0].password));
    });

    loginfo("starting network...");

    if (!services.begin(NetworkConnectionTimeoutMs, pool)) {
        logerror("error starting server");
        return;
    }

    loginfo("started");

    loginfo("waiting to serve");

    // In self AP mode we're waiting for connections now, and hold off doing
    // anything useful until something joins.
    while (services.waiting_to_serve()) {
        if (services.should_stop()) {
            return;
        }

        services.tick();

        fk_delay(10);
    }

    if (!services.can_serve()) {
        loginfo("unable to serve, retrying");
        return;
    }

    // Start the network services now that we've got things to talk to.
    if (!services.serve()) {
        logerror("error serving");
        return;
    }

    loginfo("awaiting connections...");

    gsm.apply([=](GlobalState *gs) {
        gs->network.state.enabled = fk_uptime();
        gs->network.state.connected = fk_uptime();
        gs->network.state.ip = get_network()->ip_address();
    });

    while (services.serving()) {
        if (services.should_stop()) {
            return;
        }

        services.tick();

        if (!services.active_connections()) {
            fk_delay(10);
        }
    }
}

void fk_live_tests() {
    if (false) {
        // wifi_test();
        wifi_low_level_test();

        while (true) {
            fk_delay(1000);
        }
    }
    if (false) {
        scan_i2c_module_bus();
    }
    if (false) {
        scan_i2c_radio_bus();
    }
    if (false) {
        write_headers();
    }
    if (false) {
        test_core_dump();

        while (true) {
            fk_delay(1000);
        }
    }
}

} // namespace fk
