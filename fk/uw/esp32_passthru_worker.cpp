#if defined(__SAMD51__) && defined(FK_NETWORK_ESP32)

#include "uw/esp32_passthru_worker.h"
#include "networking/wifi_toggle_worker.h"
#include "hal/pins.h"

#include <Arduino.h>

namespace fk {

FK_DECLARE_LOGGER("esp32");

Esp32PassthruWorker::Esp32PassthruWorker() {
}

void Esp32PassthruWorker::run(Pool &pool) {
    WifiToggleWorker disable_wifi{ WifiToggleWorker::DesiredState::Disabled };

    disable_wifi.run(pool);

    Serial.begin(115200);
    Serial0.begin(115200);

    pinMode(WIFI_ESP32_CS, OUTPUT);
    pinMode(WIFI_ESP32_GPIO0, OUTPUT);
    pinMode(WIFI_ESP32_RESET, OUTPUT);

    // Manually put the ESP32 in upload mode
    digitalWrite(WIFI_ESP32_CS, LOW);
    digitalWrite(WIFI_ESP32_GPIO0, LOW);
    digitalWrite(WIFI_ESP32_RESET, LOW);
    delay(100);
    digitalWrite(WIFI_ESP32_RESET, HIGH);
    delay(100);

    loginfo("beginning passthru");

    while (true) {
        while (Serial.available()) {
            SEGGER_RTT_PutChar(0, '>');
            Serial0.write(Serial.read());
        }

        while (Serial0.available()) {
            SEGGER_RTT_PutChar(0, '<');
            Serial.write(Serial0.read());
        }
    }
}

} // namespace fk

#endif