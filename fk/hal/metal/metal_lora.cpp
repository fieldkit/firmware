#include "hal/metal/metal_lora.h"
#include "hal/board.h"
#include "hal/metal/sc16is740.h"
#include "utilities.h"

#include <Arduino.h>

namespace fk {

FK_DECLARE_LOGGER("lora");

Rn2903LoraNetwork::Rn2903LoraNetwork() {
}

bool Rn2903LoraNetwork::query_status() {
    const char *line = nullptr;

    if (!rn2903_.simple_query("sys get hweui", &line, 1000)) {
        return false;
    }
    if (!rn2903_.simple_query("sys get vdd", &line, 1000)) {
        return false;
    }

    if (!rn2903_.simple_query("mac get status", &line, 1000)) {
        return false;
    }
    if (!rn2903_.simple_query("mac get appeui", &line, 1000)) {
        return false;
    }

    if (!rn2903_.simple_query("mac get deveui", &line, 1000)) {
        return false;
    }
    FK_ASSERT(hex_string_to_bytes(device_eui_, sizeof(device_eui_), line) == sizeof(device_eui_));

    if (!rn2903_.simple_query("mac get devaddr", &line, 1000)) {
        return false;
    }
    if (!rn2903_.simple_query("mac get upctr", &line, 1000)) {
        return false;
    }
    if (!rn2903_.simple_query("mac get dr", &line, 1000)) {
        return false;
    }
    if (!rn2903_.simple_query("mac get adr", &line, 1000)) {
        return false;
    }
    if (!rn2903_.simple_query("mac get rxdelay1", &line, 1000)) {
        return false;
    }
    if (!rn2903_.simple_query("mac get rxdelay2", &line, 1000)) {
        return false;
    }

    return true;
}

bool Rn2903LoraNetwork::join(const char *app_eui, const char *app_key, int32_t retries, uint32_t retry_delay) {
    if (!rn2903_.join(app_eui, app_key, retries, retry_delay)) {
        return false;
    }

    return true;
}

bool Rn2903LoraNetwork::resume_previous_session() {
    if (!rn2903_.join("abp")) {
        return false;
    }

    return true;
}

bool Rn2903LoraNetwork::save_state() {
    return rn2903_.save_state();
}

bool Rn2903LoraNetwork::power(bool on) {
    if (on) {
        logdebug("power on");
        get_board()->enable_lora();
    }
    else {
        logdebug("power off");
        get_board()->disable_lora();
    }

    return true;
}

bool Rn2903LoraNetwork::sleep(uint32_t ms) {
    return rn2903_.sleep(ms);
}

bool Rn2903LoraNetwork::wake() {
    return rn2903_.wake();
}

bool Rn2903LoraNetwork::begin() {
    status_ = Availability::Unavailable;

    if (!power(false)) {
        return false;
    }

    fk_delay(500);

    if (!power(true)) {
        return false;
    }

    fk_delay(100);

    if (!rn2903_.begin()) {
        return false;
    }

    const char *line = nullptr;
    if (!rn2903_.read_line_sync(&line, 5000)) {
        return false;
    }

    if (!query_status()) {
        return false;
    }

    status_ = Availability::Available;

    return true;
}

bool Rn2903LoraNetwork::send_bytes(uint8_t port, uint8_t const *data, size_t size, bool confirmed) {
    if (!rn2903_.send_bytes(data, size, port, confirmed)) {
        return false;
    }

    /*
    const char *line = nullptr;
    if (!rn2903_.simple_query("radio get rssi", &line, 1000)) {
        return false;
    }
    */

    return true;
}

} // namespace fk
