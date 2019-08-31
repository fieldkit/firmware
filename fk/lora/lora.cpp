#include "lora/lora.h"
#include "hal/board.h"
#include "platform.h"
#include "utilities.h"
#include "sc16is740.h"

#if defined(ARDUINO)

#include <Arduino.h>

namespace fk {

FK_DECLARE_LOGGER("lora");

enum class LoraState {
    Initializing,
};

LoraModule::LoraModule() {
}

bool LoraModule::begin() {
    pinMode(LORA_ENABLE, OUTPUT);
    digitalWrite(LORA_ENABLE, LOW);
    fk_delay(500);
    digitalWrite(LORA_ENABLE, HIGH);
    fk_delay(100);

    auto bus = get_board()->i2c_radio();

    SC16IS740 bridge{ bus };

    if (!bridge.begin()) {
        return false;
    }

    uint8_t buffer[1024];
    size_t position = 0;

    LoraState state = LoraState::Initializing;

    while (true) {
        auto nread = bridge.available_for_read();
        if (nread > 0) {
            auto can_read = std::min<size_t>(sizeof(buffer) - position, nread);
            if (!bridge.read_fifo(buffer + position, can_read)) {
                logerror("error reading fifo");
                return false;
            }
        }

        fk_delay(1000);
    }

    loginfo("done");

    return true;
}

}

#endif
