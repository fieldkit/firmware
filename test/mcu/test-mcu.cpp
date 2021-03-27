#include <os.h>

#include <Arduino.h>

void setup() {
    Serial.begin(115200);

    while (!Serial && millis() < 2000) {
        os_delay(10);
    }
}

void loop() {
}
