#include <loading.h>
#include <os.h>

#include "platform.h"
#include "self_check.h"
#include "httpd/server.h"
#include "secrets.h"

#include <Arduino.h>
#include <WiFi101.h>
#include <SPI.h>
#include <Wire.h>

using namespace fk;

static os_task_t idle_task;
static uint32_t idle_stack[OS_STACK_MINIMUM_SIZE_WORDS];

static void task_handler_idle(void *params) {
    while (true) {
        delay(1000);
        fkb_external_println("fk: ping");
    }
}

void run_tasks() {
    OS_CHECK(os_initialize());

    OS_CHECK(os_task_initialize(&idle_task, "idle", OS_TASK_START_RUNNING, &task_handler_idle, NULL, idle_stack, sizeof(idle_stack)));

    OS_CHECK(os_start());
}

/**
 * RADIO_MOSI  PB26
 * RADIO_MISO  PB29
 * RADIO_SCK   PB27
 *
 * WIFI_CS     PB05
 * WIFI_ENABLE PC25
 * WIFI_RESET  PC26
 * WIFI_IRQ    PC27
 *
 * WIFI_POWER  PB07
 *
 * GPS_TX      PB24
 * GPS_RX      PB25
 * GPS_POWER   PB06
 *
 * Wire1
 * RAD_SDA     PC22
 * RAD_SCL     PC23
 */

constexpr uint8_t GPS_POWER = 55u;

void gps() {
    fkb_external_println("fk: checking gps");

    auto working = false;

    uint32_t started = fk_uptime();
    while ((fk_uptime() - started) < 5000) {
        if (Serial1.available()) {
            auto c = Serial1.read();
            SEGGER_RTT_PutChar(0, c);
            working = true;
        }
    }

    if (working) {
        fkb_external_println("fk: gps found!");
    }
    else {
        fkb_external_println("fk: no gps!");
    }
}

void wifi() {
    fkb_external_println("fk: checking wifi");

    SPI1.begin();

    WiFi.setPins(WINC1500_CS, WINC1500_IRQ, WINC1500_RESET);

    if (WiFi.status() == WL_NO_SHIELD) {
        fkb_external_println("fk: no wifi");
        return;
    }

    WiFi.end();

    fkb_external_println("fk: wifi found!");
}

uint16_t read_u16(uint8_t address, uint8_t reg) {
    Wire1.beginTransmission(address);
    Wire1.write(reg);
    Wire1.endTransmission();
    Wire1.requestFrom(address, 2);
    uint16_t value  = Wire1.read();
    value |= (uint16_t)Wire1.read() << 8;
    return value;
}

bool write_u16(uint8_t address, uint8_t reg, uint16_t value) {
    Wire1.beginTransmission(address);
    Wire1.write(reg);
    Wire1.write( value       & 0xFF);
    Wire1.write((value >> 8) & 0xFF);
    return Wire1.endTransmission() == 0;
}

void fuel_gauge() {
    Wire1.begin();

    for (auto i = 0; i < 0x128; ++i) {
        auto address = 0x36;

        if (!write_u16(address, 0x18, 2000 * 2)) {
            fkb_external_println("fk: failed to write capacity");
            break;
        }

        auto raw_voltage = read_u16(address, 0x09);
        auto voltage = (float)raw_voltage * 7.8125e-5f;

        fkb_external_println("fk: raw voltage=%d", raw_voltage);
        fkb_external_println("fk: %f", voltage);

        break;
    }
}

void setup() {
    fkb_external_println("fk: hello!");

    if (false) {
        pinMode(PIN_WIRE1_SDA, OUTPUT);
        pinMode(PIN_WIRE1_SCL, OUTPUT);

        while (true) {
            digitalWrite(PIN_WIRE1_SDA, LOW);
            digitalWrite(PIN_WIRE1_SCL, LOW);

            delay(1000);

            digitalWrite(PIN_WIRE1_SDA, HIGH);
            digitalWrite(PIN_WIRE1_SCL, HIGH);

            delay(1000);
        }
    }

    pinMode(WINC1500_CS, OUTPUT);
    pinMode(WINC1500_IRQ, INPUT);
    pinMode(WINC1500_RESET, OUTPUT);

    pinMode(GPS_POWER, OUTPUT);
    digitalWrite(GPS_POWER, LOW);
    delay(100);
    digitalWrite(GPS_POWER, HIGH);
    delay(100);

    pinMode(WINC1500_POWER, OUTPUT);
    digitalWrite(WINC1500_POWER, LOW);
    delay(100);
    digitalWrite(WINC1500_POWER, HIGH);
    delay(100);

    Serial1.begin(9600);

    DisplayFactory display_factory;
    Display *display = display_factory.get_display();
    SelfCheck self_check(display);

    self_check.check();

    delay(1000);

    display->fk_logo();

    delay(1000);

    #if defined(FK_WIFI_0_SSID) && defined(FK_WIFI_0_PASSWORD)
    HttpServer http_server{ FK_WIFI_0_SSID, FK_WIFI_0_PASSWORD };
    #else
    HttpServer http_server{ nullptr, nullptr };
    #endif

    if (!http_server.begin()) {
        fkb_external_println("fk: Wifi missing");
        while (true) {
        }
    }

    while (true) {
        home_screen_t screen = {
            .time = fk_uptime(),
            .wifi = true,
            .gps = true,
            .battery = 1.0f,
        };

        display->home(screen);

        http_server.tick();

        delay(10);
    }

    run_tasks();
}

void loop() {
}
