#include "common.h"
#include "platform.h"
#include "httpd/server.h"

#include <SPI.h>

namespace fk {

static bool wifi_ready() {
    return WiFi.status() == WL_CONNECTED || WiFi.status() == WL_AP_LISTENING;
}

HttpServer::HttpServer() : ssid_(nullptr), password_(nullptr) {
}

HttpServer::HttpServer(const char *ssid, const char *password) : ssid_(ssid), password_(password) {
}

bool HttpServer::begin() {
    SPI1.begin();

    WiFi.setPins(WINC1500_CS, WINC1500_IRQ, WINC1500_RESET);

    if (WiFi.status() == WL_NO_SHIELD) {
        fkb_external_println("fk: no wifi");
        return false;
    }

    const char *name = "FK-DARWIN";

    fkb_external_println("fk: establishing wifi...");

    while (!wifi_ready()) {
        if (ssid_ == nullptr) {
            WiFi.beginAP(name);
        }
        else {
            WiFi.begin(ssid_, password_);
        }

        auto started = fk_uptime();
        while (fk_uptime() - started < 10 * 1000 && !wifi_ready()) {
            delay(100);
        }
    }

    server_.begin();

    IPAddress ip = WiFi.localIP();

    fkb_external_println("fk: connected (ip = %d.%d.%d.%d)", ip[0], ip[1], ip[2], ip[3]);

    if (!mdns_.begin(WiFi.localIP(), name)) {
        fkb_external_println("fk: unable to start mdns responder!");
        return false;
    }

    mdns_.addServiceRecord("station._fk", 80, MDNSServiceTCP);

    return true;
}

static uint32_t available = true;
static uint32_t changed = 0;

void HttpServer::tick() {
    mdns_.run();

    if ((fk_uptime() - changed) > 30 * 1000) {
        if (!available) {
            mdns_.addServiceRecord("station._fk", 80, MDNSServiceTCP);
        }
        else {
            mdns_.removeServiceRecord(80, MDNSServiceTCP);
        }
        available = !available;
        changed = fk_uptime();
    }

    auto wcl = server_.available();
    if (!wcl) {
        return;
    }

    fkb_external_println("fk: connection!");

    uint8_t buffer[1024] = { 0 };
    size_t position = 0;
    HttpRequest req;

    // TODO: 414 Request-URI Too Long
    // TODO: 431 Request Header Fields Too Large.
    // TODO: 413 Payload Too Large
    // TODO: 500 Service Unavailable
    // TODO: 503 Service Unavailable

    while (wcl.connected()) {
        if (wcl.available()) {
            auto available = sizeof(buffer) - position;
            auto nread = wcl.read(buffer, available);
            if (nread < 0) {
                fkb_external_println("fk: EOS read");
                continue;
            }
            if (nread == 0) {
                fkb_external_println("fk: empty read");
                continue;
            }

            if (req.parse((const char *)(buffer + position), nread) != 0) {
            }

            if (req.have_headers()) {
            }

            if (req.consumed()) {
                wcl.println("HTTP/1.1 204 OK");
                wcl.println("Content-Length: 0");
                wcl.println("Content-Type: text/html");
                wcl.println("Connection: close");
                wcl.println();
                break;
            }

            position += nread;
        }
    }

    wcl.stop();
}

}
