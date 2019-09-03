#include <tiny_printf.h>

#include "rn2903.h"
#include "utilities.h"

#if defined(ARDUINO)

namespace fk {

FK_DECLARE_LOGGER("lora");

#define TTN_DEFAULT_SF  7
#define TTN_DEFAULT_FSB 2

Rn2903::Rn2903() {
}

bool Rn2903::begin() {
    return bridge_.begin();
}

bool Rn2903::sleep(uint32_t ms) {
    if (!send_command("sys sleep %d", ms)) {
        return false;
    }

    return true;
}

bool Rn2903::wake() {
    uint8_t buffer[] = {
        0x00,
        0x55,
        '\r',
        '\n',
    };

    for (auto i = 0; i < 10; ++i) {
        fk_delay(100);

        if (bridge_.write_fifo(buffer, sizeof(buffer))) {
            const char *line = nullptr;

            // This should be 'ok', which is the ack from the actual sleep.
            if (read_line_sync(&line, 1000)) {
                loginfo("rn2903 > '%s'", line);

                line_reader_.read_line_sync(&line, 1000);
                line_reader_.clear();

                return true;
            }
        }
    }

    return false;
}

bool Rn2903::read_line_sync(const char **line, uint32_t to) {
    if (!line_reader_.read_line_sync(line, to)) {
        logerror("error reading line");
        return false;
    }

    return true;
}

bool Rn2903::send_command(const char *cmd, ...) {
    va_list args;
    va_start(args, cmd);
    auto rv = send_command(cmd, args);
    va_end(args);

    return rv;
}

bool Rn2903::send_command(const char *cmd, va_list args) {
    char buffer[256];
    auto needed = tiny_vsnprintf(buffer, sizeof(buffer) - 3, cmd, args);
    FK_ASSERT(needed + 3 < (int32_t)sizeof(buffer));

    loginfo("rn2903 < '%s'", buffer);

    buffer[needed + 0] = '\r';
    buffer[needed + 1] = '\n';
    buffer[needed + 2] = 0;

    if (!bridge_.write(buffer)) {
        logerror("error writing line");
        return false;
    }

    return true;
}

bool Rn2903::simple_query(const char *cmd, uint32_t to, ...) {
    va_list args;
    va_start(args, to);
    auto rv = send_command(cmd, args);
    va_end(args);

    if (!rv) {
        return false;
    }

    const char *line = nullptr;
    if (!read_line_sync(&line, to)) {
        return false;
    }

    if (strncmp(line, "ok", 2) != 0) {
        loginfo("rn2903 > '%s'", line);
        return false;
    }

    return true;
}

bool Rn2903::simple_query(const char *cmd, const char **line, uint32_t to, ...) {
    va_list args;
    va_start(args, to);
    auto rv = send_command(cmd, args);
    va_end(args);

    if (!rv) {
        return false;
    }

    if (!read_line_sync(line, to)) {
        return false;
    }

    loginfo("rn2903 > '%s'", *line);

    return true;
}

bool Rn2903::save_state() {
    if (!simple_query("mac save", 1000)) {
        return false;
    }

    return true;
}

bool Rn2903::provision(const char *app_eui, const char *app_key) {
    if (strlen(app_eui) != 16 || strlen(app_key) != 32) {
        logerror("malformed app_eui and/or app_key");
        return false;
    }

    const char *line = nullptr;
    if (!simple_query("sys get hweui", &line, 1000)) {
        return false;
    }

    char hweui[64];
    strncpy(hweui, line, sizeof(hweui));

    if (!simple_query("mac set deveui %s", 1000, hweui)) {
        return false;
    }

    if (!simple_query("mac set appeui %s", 1000, app_eui)) {
        return false;
    }

    if (!simple_query("mac set appkey %s", 1000, app_key)) {
        return false;
    }

    // NOTE: This is required anytime you save parameters. Per the RN2903
    // manual, If this parameter was previously saved to user EEPROM by issuing
    // the mac save command, after modifying its value, the mac save command
    // should be called again.
    // https://www.loraserver.io/lora-app-server/use/devices/#to-set-the-appeui-and-appkey
    if (!simple_query("mac set devaddr %s", &line, 1000, "00000000")) {
        return false;
    }

    if (!save_state()) {
        return false;
    }

    return true;
}

bool Rn2903::configure_us915(uint8_t fsb) {
    uint8_t ch_low = fsb > 0 ? (fsb - 1) * 8 : 0;
    uint8_t ch_high = fsb > 0 ? ch_low + 7 : 71;
    uint8_t ch_500 = fsb + 63;
    for (uint8_t ch = 0; ch < 72; ch++) {
        if (ch == ch_500 || (ch <= ch_high && ch >= ch_low)) {
            if (!simple_query("mac set ch status %d on", 1000, ch)) {
                return false;
            }
            if (ch < 63) {
                if (!simple_query("mac set ch drrange %d 0 3", 1000, ch)) {
                    return false;
                }
            }
        }
        else {
            if (!simple_query("mac set ch status %d off", 1000, ch)) {
                return false;
            }
        }
    }
    if (!simple_query("mac set pwridx 5", 1000)) {
        return false;
    }
    if (!simple_query("mac set retx 7", 1000)) {
        return false;
    }
    return true;
}

bool Rn2903::configure_sf(uint8_t sf) {
    // NOTE This needs to work with other frequencies.
    auto dr = 10 - sf;
    if (!simple_query("mac set dr %d", 1000, dr)) {
        return false;
    }

    return true;
}

bool Rn2903::join(const char *app_eui, const char *app_key, int32_t retries, uint32_t retry_delay) {
    if (!provision(app_eui, app_key)) {
        return false;
    }

    // NOTE This needs to work with other frequencies.
    if (!configure_us915(TTN_DEFAULT_FSB)) {
        return false;
    }
    if (!configure_sf(TTN_DEFAULT_SF)) {
        return false;
    }

    int8_t tries = 0;
    while (retries == -1 || tries <= retries) {
        if (tries > 0) {
            logwarn("join failed, waiting...");
            fk_delay(retry_delay);
        }

        tries++;

        if (!simple_query("mac join otaa", 1000)) {
            continue;
        }

        const char *line = nullptr;

        if (!read_line_sync(&line, 10000)) {
            return false;
        }

        loginfo("rn2903 > '%s'", line);

        if (strstr(line, "accepted") == nullptr) {
            continue;
        }

        if (!simple_query("mac get devaddr", &line, 1000)) {
            return false;
        }

        return true;
    }

    return false;
}

bool Rn2903::send_bytes(uint8_t const *data, size_t size, uint8_t port) {
    char hex[size * 2 + 1];
    bytes_to_hex_string(hex, sizeof(hex), data, size);

    const char *mode = "cnf";
    if (!simple_query("mac tx %s %d %s", 1000, mode, port, hex)) {
        return false;
    }

    auto started = fk_uptime();

    const char *line = nullptr;
    if (!read_line_sync(&line, 60000)) {
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

#endif
