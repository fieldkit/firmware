#include "hal/metal/metal_lora.h"
#include "hal/board.h"
#include "hal/metal/sc16is740.h"
#include "utilities.h"

#include <Arduino.h>

namespace fk {

FK_DECLARE_LOGGER("lora");

Rn2903LoraNetwork::Rn2903LoraNetwork() {
}

bool Rn2903LoraNetwork::show_status() {
    const char *line = nullptr;
    if (!rn2903_.simple_query("sys get hweui", &line, 1000)) {
        return false;
    }
    if (!rn2903_.simple_query("sys get vdd", &line, 1000)) {
        return false;
    }
    if (!rn2903_.simple_query("mac get appeui", &line, 1000)) {
        return false;
    }
    if (!rn2903_.simple_query("mac get deveui", &line, 1000)) {
        return false;
    }
    if (!rn2903_.simple_query("mac get dr", &line, 1000)) {
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

bool Rn2903LoraNetwork::power(bool on) {
    if (on) {
        get_board()->enable_lora();
    }
    else {
        get_board()->disable_lora();
    }

    return true;
}

bool Rn2903LoraNetwork::sleep(uint32_t ms) {
    if (!rn2903_.simple_query("sys sleep %d", 1000)) {
        return false;
    }

    return true;
}

bool Rn2903LoraNetwork::begin() {
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

    if (!show_status()) {
        return false;
    }

    const char *app_eui = "0000000000000000";
    const char *app_key = "39e98dbaa08feed53d5f68d43d0ef981";

    if (!rn2903_.join(app_eui, app_key)) {
        return false;
    }

    return true;
}

bool Rn2903LoraNetwork::send_bytes(uint8_t const *data, size_t size) {
    uint8_t port = 10;
    char hex[size * 2 + 1];
    bytes_to_hex_string(hex, sizeof(hex), data, size);

    const char *mode = "cnf";
    if (!rn2903_.simple_query("mac tx %s %d %s", 1000, mode, port, hex)) {
        return false;
    }

    auto started = fk_uptime();

    const char *line = nullptr;
    if (!rn2903_.read_line_sync(&line, 60000)) {
        return false;
    }

    loginfo("rn2903 > '%s' (%" PRIu32 "ms)", line, fk_uptime() - started);

    const char *mac_tx_ok = "mac_tx_ok";
    if (strncmp(line, mac_tx_ok, strlen(mac_tx_ok)) == 0) {
        return true;
    }

    const char *mac_rx = "mac_rx";
    if (strncmp(line, mac_rx, strlen(mac_rx)) == 0) {
        return true;
    }

    return false;
}

}
