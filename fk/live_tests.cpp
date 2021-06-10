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
#include "clock.h"
#include "networking/cpool.h"
#include "networking/default_routes.h"

#include "../modules/weather/main/weather.h"

extern const struct fkb_header_t fkb_header;

extern "C" {

// From wifi101 hif
extern void fkwifi_log();

uint32_t packet_writes = 0;

}

namespace fk {

FK_DECLARE_LOGGER("live-tests");

static void scan_i2c_radio_bus() __attribute__((unused));

static void scan_i2c_module_bus() __attribute__((unused));

static void scan_i2c_radio_bus() {
    auto bus = get_board()->i2c_radio();

    bus.begin();

    get_board()->enable_lora();

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

    mm->disable_all_modules();

    fk_delay(1000);

    mm->enable_all_modules();

    fk_delay(100);

    auto bus = get_board()->i2c_module();

    while (true) {
        for (auto i : { ModulePosition::from(3) }) {
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
    StandardPool pool{ "write-headers "};

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

void test_networking() {
#if defined(FK_WIFI_0_SSID) && defined(FK_WIFI_0_PASSWORD)
    StandardPool main_pool{ "network-test" };

    get_board()->enable_everything();

    auto network = get_network();

    network->begin(
        {
            .valid = true,
            .create = false,
            .ssid = FK_WIFI_0_SSID,
            .password = FK_WIFI_0_PASSWORD,
        },
        &main_pool);

    while (network->status() != NetworkStatus::Connected) {
        fk_delay(1000);
        loginfo("connecting...");
    }

    loginfo("connected!");

    if (true) {
        HttpRouter router;
        ConnectionPool connection_pool{ router };

        DefaultRoutes default_routes;
        default_routes.add_routes(router);

        auto http_listener = network->listen(80);

        if (!network->serve()) {
            logerror("serving");
            while (true) {
                fk_delay(100);
            }
        }

        while (true) {
            if (connection_pool.available() > 0) {
                auto http_connection = http_listener->get()->accept();
                if (http_connection != nullptr) {
                    connection_pool.queue_http(http_connection);
                }
            }

            connection_pool.service();
        }
    }

    if (false) {
        while (true) {
            StandardPool pool{ "test" };
            auto url = "https://api.fkdev.org/status";
            auto http = open_http_connection("GET", url, "", true, pool);
            if (http != nullptr) {
                http->close();
            }

            fk_delay(1000);

            loginfo("ping");
        }
    }
#endif

    while (true) {
        fk_delay(1000);
    }
}

void wifi101_example() {
#if defined(__SAMD51__)
#if defined(FK_WIFI_0_SSID) && defined(FK_WIFI_0_PASSWORD)
    StandardPool pool{ "wifi101-examplel" };
    char ssid[] = FK_WIFI_0_SSID;
    char pass[] = FK_WIFI_0_PASSWORD;

    int32_t status = WL_IDLE_STATUS;

    StaticWiFiCallbacks staticWiFiCallbacks;

    staticWiFiCallbacks.initialize(pool);

    WiFiSocketClass::callbacks = &staticWiFiCallbacks;

    get_board()->enable_wifi();

    fk_delay(100);

    WiFi.setPins(WINC1500_CS, WINC1500_IRQ, WINC1500_RESET);

    WiFiServer server(80);

    // check for the presence of the shield:
    if (WiFi.status() == WL_NO_SHIELD) {
        logerror("WiFi shield not present");
        while (true);
    }

    status = WiFi.begin(ssid, pass);

    while (status != WL_CONNECTED) {
        delay(1000);
        logdebug("connecting");
    }

    server.begin();

    SEGGER_RTT_printf(0, "ready\n");

    uint32_t total_transferred = 0;

    while (true) {
        StandardPool loop_pool{ "wifi101-loop" };

        auto wcl = server.available();
        if (wcl) {
            SEGGER_RTT_printf(0, "new connection\n");

            auto total_bytes = 1024u * 1024u * 100u;
            String line = "";
            while (wcl.connected()) {
                if (wcl.available()) {
                    char c = wcl.read();
                    if (c == '\n') {
                        if (line.length() == 0) {
                            auto buffer_size = 1400;
                            auto buffer = (uint8_t *)loop_pool.malloc(buffer_size);
                            memset(buffer, 0, buffer_size);

                            strcpy((char *)buffer, "HTTP/1.1 200 OK\n");
                            strcat((char *)buffer, "Content-Type: text/html\n");
                            strcat((char *)buffer, loop_pool.sprintf("Content-Length: %d\n\n", total_bytes));

                            wcl.write(buffer, strlen((char *)buffer));

                            memset(buffer, 0, buffer_size);

                            char message[128];

                            struct header_t {
                                uint32_t packets;
                                uint32_t counter;
                            };

                            uint32_t counter = 0;
                            uint8_t marker = 0;
                            for (auto copied = 0u; copied < total_bytes; ) {
                                memset(buffer, marker++, buffer_size);

                                header_t header;
                                header.packets = packet_writes++;
                                header.counter = counter++;

                                tiny_snprintf(message, sizeof(message), "   packets=%" PRIu32 " counter=%" PRIu32 "   ", packet_writes, counter);

                                memcpy((void *)(buffer), &header, sizeof(header));
                                memcpy((void *)(buffer + sizeof(header_t)), message, strlen(message));

                                auto write_started = fk_uptime();
                                auto bytes_copied = wcl.write(buffer, buffer_size);
                                auto write_took = fk_uptime() - write_started;

                                auto flush_started = fk_uptime();
                                wcl.flush();
                                auto flush_took = fk_uptime() - flush_started;

                                if (bytes_copied == 0) {
                                    loginfof("live", "packets=%" PRIu32 " copied=%d bytes=%d wrote=%" PRIu32 "ms flush=%" PRIu32 "ms rssi=%" PRIu32 " (failed)",
                                                      packet_writes, copied, bytes_copied, write_took, flush_took, WiFi.RSSI());
                                    fk_delay(100);
                                } else {
                                    loginfof("live", "packets=%" PRIu32 " copied=%d bytes=%d wrote=%" PRIu32 "ms flush=%" PRIu32 "ms rssi=%" PRIu32 " total=%" PRIu32 "",
                                             packet_writes, copied, bytes_copied, write_took, flush_took, WiFi.RSSI(),
                                             total_transferred);
                                    fkwifi_log();
                                    copied += bytes_copied;
                                    total_transferred += bytes_copied;
                                }
                            }
                            break;
                        }
                        else {
                            line = "";
                        }
                    }
                    else if (c != '\r') {
                        line += c;
                    }
                }
            }

            wcl.stop();

            SEGGER_RTT_printf(0, "bye bye\n");
        }

        fk_delay(100);
    }
#endif
#endif

    while (true) {
        fk_delay(1000);
    }
}

os_task_t test_idle_task;
os_task_t test_work_task;

void test_handler_idle(void *params) {
    while (true) {
        fk_delay(FiveSecondsMs);
    }
}

void test_handler_work(void *params) {
    wifi101_example();
}

void live_os() {
    OS_CHECK(os_initialize());

    uint32_t idle_stack[1024];
    uint32_t work_stack[4096];
    OS_CHECK(os_task_initialize(&test_idle_task, "idle", OS_TASK_START_RUNNING, &test_handler_idle, nullptr, idle_stack, sizeof(idle_stack)));
    OS_CHECK(os_task_initialize(&test_work_task, "work", OS_TASK_START_RUNNING, &test_handler_work, nullptr, work_stack, sizeof(work_stack)));

    FK_ASSERT(get_ipc()->begin());

    OS_CHECK(os_start());
}

void fk_live_tests() {
    if (false) {
        scan_i2c_module_bus();
    }
    if (false) {
        scan_i2c_radio_bus();
    }
    if (false) {
        write_headers();
        while (true) {
            fk_delay(1000);
        }
    }
#if defined(FK_DEBUG_LIVE_TEST_ENABLE)
    FK_DEBUG_LIVE_TEST_ENABLE();
#endif
}

}
