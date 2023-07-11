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

#if defined(__SAMD51__)
#include <WiFi101.h>

#if !defined(FK_NETWORK_ESP32)
#include <driver/include/m2m_types.h>
#include <driver/include/m2m_ota.h>
#endif
#endif

extern const struct fkb_header_t fkb_header;

namespace fk {

FK_DECLARE_LOGGER("live-tests");

static void scan_i2c_radio_bus() __attribute__((unused));

static void scan_i2c_module_bus() __attribute__((unused));

static void wifi_low_level_test() __attribute__((unused));

static void write_headers() __attribute__((unused));

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

static void wifi_low_level_test() {
#if defined(FK_NETWORK_ESP32) && defined(FK_WIFI_0_SSID) && defined(FK_WIFI_0_PASSWORD)
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
        status = WiFi.begin(FK_WIFI_0_SSID, FK_WIFI_0_PASSWORD);
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

#if defined(FK_LIVE_TEST_WIFI) && defined(FK_WIFI_0_SSID) && defined(FK_WIFI_0_PASSWORD)
static void wifi_test() __attribute__((unused));

static void wifi_test() {
    StandardPool pool{ "network-task" };
    NetworkServices services{ get_network(), pool };
    GlobalStateManager gsm;

    gsm.apply([=](GlobalState *gs) {
        gs->network.config.wifi_networks[0] = {};
        gs->network.config.wifi_networks[0].valid = true;
        strncpy(gs->network.config.wifi_networks[0].ssid, FK_WIFI_0_SSID, sizeof(gs->network.config.wifi_networks[0].ssid));
        strncpy(gs->network.config.wifi_networks[0].password, FK_WIFI_0_PASSWORD, sizeof(gs->network.config.wifi_networks[0].password));
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
#endif

#if defined(FK_LIVE_TEST_WINC1500_OTA) && !defined(FK_NETWORK_ESP32)
#define MAIN_WLAN_SSID FK_WIFI_0_SSID
#define MAIN_WLAN_AUTH M2M_WIFI_SEC_WPA_PSK
#define MAIN_WLAN_PSK  FK_WIFI_0_PASSWORD
#define MAIN_OTA_URL   "http://192.168.0.100:8000/m2m_ota_3a0.bin"

static void wifi_cb(uint8_t u8MsgType, void *pvMsg) {
    switch (u8MsgType) {
    case M2M_WIFI_RESP_CON_STATE_CHANGED: {
        tstrM2mWifiStateChanged *pstrWifiState = (tstrM2mWifiStateChanged *)pvMsg;
        if (pstrWifiState->u8CurrState == M2M_WIFI_CONNECTED) {
            m2m_wifi_request_dhcp_client();
        } else if (pstrWifiState->u8CurrState == M2M_WIFI_DISCONNECTED) {
            loginfo("Wi-Fi disconnected");

            /* Connect to defined AP. */
            m2m_wifi_connect((char *)MAIN_WLAN_SSID, sizeof(MAIN_WLAN_SSID), MAIN_WLAN_AUTH, (void *)MAIN_WLAN_PSK, M2M_WIFI_CH_ALL);
        }

        break;
    }

    case M2M_WIFI_REQ_DHCP_CONF: {
        uint8_t *pu8IPAddress = (uint8_t *)pvMsg;
        loginfo("Wi-Fi connected");
        loginfo("Wi-Fi IP is %u.%u.%u.%u", pu8IPAddress[0], pu8IPAddress[1], pu8IPAddress[2], pu8IPAddress[3]);
        m2m_ota_start_update((uint8_t *)MAIN_OTA_URL);
        break;
    }

    default: {
        break;
    }
    }
}

static void OtaUpdateCb(uint8_t u8OtaUpdateStatusType, uint8_t u8OtaUpdateStatus) {
    loginfo("OtaUpdateCb %d %d", u8OtaUpdateStatusType, u8OtaUpdateStatus);
    if (u8OtaUpdateStatusType == DL_STATUS) {
        if (u8OtaUpdateStatus == OTA_STATUS_SUCCESS) {
            /* Start Host Controller OTA HERE ... Before switching.... */
            loginfo("OtaUpdateCb m2m_ota_switch_firmware start.");
            m2m_ota_switch_firmware();
        } else {
            loginfo("OtaUpdateCb FAIL u8OtaUpdateStatus: %d", u8OtaUpdateStatus);
        }
    } else if (u8OtaUpdateStatusType == SW_STATUS) {
        if (u8OtaUpdateStatus == OTA_STATUS_SUCCESS) {
            loginfo("OTA Success. Press reset your board.");
            /* system_reset(); */
        }
    }
}

static void OtaNotifCb(tstrOtaUpdateInfo *pv) {
    loginfo("OtaNotifCb");
}

static void wifi_firmware_upgrade() __attribute__((unused));

static void wifi_firmware_upgrade() {
    tstrWifiInitParam param;
    int8_t ret;

    pinMode(WINC1500_CS, OUTPUT);
    pinMode(WINC1500_IRQ, INPUT);
    pinMode(WINC1500_RESET, OUTPUT);

    digitalWrite(WINC1500_POWER, HIGH);
    SPI1.begin();

    WiFi.setPins(WINC1500_CS, WINC1500_IRQ, WINC1500_RESET);
    NVIC_SetPriority(EIC_11_IRQn, OS_IRQ_PRIORITY_SYSTICK - 1);

    /* Initialize the BSP. */
    nm_bsp_init();

    /* Initialize Wi-Fi parameters structure. */
    memset((uint8_t *)&param, 0, sizeof(tstrWifiInitParam));

    /* Initialize Wi-Fi driver with data and status callbacks. */
    param.pfAppWifiCb = wifi_cb;
    ret = m2m_wifi_init(&param);
    if (M2M_SUCCESS != ret) {
        loginfo("main: m2m_wifi_init call error!(%d)\r\n", ret);
        while (1) {
        }
    }

    /* Connect to defined AP. */
    m2m_wifi_connect((char *)MAIN_WLAN_SSID, sizeof(MAIN_WLAN_SSID), MAIN_WLAN_AUTH, (void *)MAIN_WLAN_PSK, M2M_WIFI_CH_ALL);

    /* Init ota function. */
    m2m_ota_init(OtaUpdateCb, OtaNotifCb);

    while (1) {
        /* Handle pending events from network controller. */
        while (m2m_wifi_handle_events(NULL) != M2M_SUCCESS) {
        }
    }
}

void wifi_server() {
    {
        StandardPool pool{ "wifi-server" };
        GlobalStateManager gsm;
        gsm.initialize(pool);

        ModuleRegistry registry;
        registry.initialize();
    }

    wifi_test();
}
#endif

void fk_live_tests() {
#if defined(FK_LIVE_TEST_WINC1500_OTA)
#if !defined(FK_NETWORK_ESP32)
    wifi_firmware_upgrade();
#endif

    while (true) {
        fk_delay(1000);
    }
#endif

#if defined(FK_LIVE_TEST_WIFI)
#if !defined(FK_NETWORK_ESP32)
    wifi_server();
#endif

    while (true) {
        fk_delay(1000);
    }
#endif
}

} // namespace fk
