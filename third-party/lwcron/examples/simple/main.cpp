#include <Arduino.h>
#include <lwcron/lwcron.h>

void setup() {
    Serial.begin(119200);

    while (!Serial) {
        delay(100);
    }
}

void loop() {

}
