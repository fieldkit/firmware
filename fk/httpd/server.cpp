#include "common.h"
#include "platform.h"
#include "httpd/server.h"

#include <SPI.h>

namespace fk {

HttpServer::HttpServer(const char *ssid, const char *password) : ssid_(ssid), password_(password) {
}

bool HttpServer::begin() {
    SPI1.begin();

    WiFi.setPins(WINC1500_CS, WINC1500_IRQ, WINC1500_RESET);

    if (WiFi.status() == WL_NO_SHIELD) {
        fkb_external_println("fk: no wifi");
        return false;
    }

    fkb_external_println("fk: connecting to AP...");

    while (WiFi.status() != WL_CONNECTED) {
        WiFi.begin(ssid_, password_);

        auto started = fk_uptime();
        while (fk_uptime() - started < 10 * 1000 && WiFi.status() != WL_CONNECTED) {
            delay(100);
        }
    }

    server_.begin();

    IPAddress ip = WiFi.localIP();

    fkb_external_println("fk: connected (ip = %d.%d.%d.%d)", ip[0], ip[1], ip[2], ip[3]);

    return true;
}

void HttpServer::tick() {
    auto wcl = server_.available();
    if (!wcl) {
        return;
    }

    fkb_external_println("fk: connection!");

    uint8_t buffer[1024] = { 0 };
    size_t position = 0;
    HttpRequest req;

    while (wcl.connected()) {
        if (wcl.available()) {
            auto available = sizeof(buffer) - position;
            auto nread = wcl.read(buffer, available);
            if (nread < 0) {
                continue;
            }

            if (req.parse((const char *)(buffer + position), nread) != 0) {
                // ERROR
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
