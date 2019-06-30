#include <loading.h>
#include <os.h>

#include "platform.h"
#include "self_check.h"
#include "httpd/server.h"
#include "secrets.h"
#include "hal/hal.h"

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
        fkinfo("ping");
    }
}

void run_tasks() {
    OS_CHECK(os_initialize());

    OS_CHECK(os_task_initialize(&idle_task, "idle", OS_TASK_START_RUNNING, &task_handler_idle, NULL, idle_stack, sizeof(idle_stack)));

    OS_CHECK(os_start());
}

size_t write_log(const LogMessage *m, const char *line) {
    return fkb_external_printf("%06" PRIu32 " %s: %s\n", m->uptime, m->facility, m->message);
}

void setup() {
    log_configure_writer(write_log);

    fkinfo("hello!");

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

    MetalWifi wifi;
    DisplayFactory display_factory;
    Display *display = display_factory.get_display();
    SelfCheck self_check(display, &wifi);

    self_check.check();

    delay(1000);

    display->fk_logo();

    delay(1000);

    #if defined(FK_WIFI_0_SSID) && defined(FK_WIFI_0_PASSWORD)
    HttpServer http_server{ &wifi, FK_WIFI_0_SSID, FK_WIFI_0_PASSWORD };
    #else
    HttpServer http_server{ &wifi };
    #endif

    auto last_changed = 0;

    while (true) {
        home_screen_t screen = {
            .time = fk_uptime(),
            .wifi = true,
            .gps = true,
            .battery = 1.0f,
        };

        if (last_changed == 0 || fk_uptime() - last_changed > 60 * 1000) {
            if (http_server.enabled()) {
                http_server.stop();
            }
            else {
                if (!http_server.begin()) {
                    fkerror("wifi missing");
                }
            }
            last_changed = fk_uptime();
        }

        display->home(screen);

        http_server.tick();

        delay(10);
    }

    run_tasks();
}

void loop() {
}
