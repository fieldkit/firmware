#include <tiny_printf.h>

#include "rn2903.h"
#include "utilities.h"

#if defined(__SAMD51__)

namespace fk {

constexpr uint32_t DefaultTimeout = 10000;

FK_DECLARE_LOGGER("lora");

#define TTN_US915_DEFAULT_SF  7
#define TTN_US915_DEFAULT_FSB 2

Rn2903::Rn2903() : bridge_(get_board()->acquire_i2c_radio()) {
}

bool Rn2903::begin() {
    return bridge_.begin(57600);
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
            if (read_line_sync(&line, DefaultTimeout)) {
                if (strncmp(line, "ok", 2) != 0) {
                    loginfo("rn2903 > '%s' (!OK)", line);
                } else {
                    loginfo("rn2903 > '%s'", line);
                }

                fk_delay(500);

                line_reader_.clear();

                return true;
            }
        }
    }

    return false;
}

bool Rn2903::factory_reset() {
    const char *line = nullptr;

    auto fr = simple_query("sys factoryRESET", &line, 5000);
    if (!fr) {
        return false;
    }

    if (strncmp(line, "RN", 2) != 0) {
        error_ = translate_error(line);
        loginfo("rn2903 > '%s' (%d)", line, error_);
        return false;
    }

    return true;
}

bool Rn2903::read_line_sync(const char **line, uint32_t to, bool quiet) {
    if (!line_reader_.read_line_sync(line, to)) {
        if (!quiet) {
            logerror("error reading line");
        }
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
    // Empty anything that's pending. Consider a delay here to be even more
    // sure that nothing's heading our way?
    bridge_.drain();

    char buffer[StackBufferSize];
    auto needed = tiny_vsnprintf(buffer, sizeof(buffer) - 3, cmd, args);
    FK_ASSERT(needed + 3 < (int32_t)sizeof(buffer));

    error_ = LoraErrorCode::None;

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
        error_ = translate_error(line);
        loginfo("rn2903 > '%s' (%d)", line, error_);
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
    if (!simple_query("mac save", DefaultTimeout)) {
        return false;
    }

    return true;
}

bool Rn2903::disable_adr() {
    if (!simple_query("mac set adr off", DefaultTimeout)) {
        return false;
    }

    return true;
}

bool Rn2903::provision(LoraOtaaJoin &otaa) {
    if (strlen(otaa.join_eui) != LoraJoinEuiLength * 2) {
        logerror("malformed join_eui");
        return false;
    }

    if (strlen(otaa.app_key) != LoraAppKeyLength * 2) {
        logerror("malformed app_key");
        return false;
    }

    if (!simple_query("mac set appeui %s", DefaultTimeout, otaa.join_eui)) {
        return false;
    }

    if (!simple_query("mac set appkey %s", DefaultTimeout, otaa.app_key)) {
        return false;
    }

    if (false) {
        const char *line = nullptr;
        if (!simple_query("sys get hweui", &line, DefaultTimeout)) {
            return false;
        }

        char hweui[64];
        strncpy(hweui, line, sizeof(hweui));

        if (!simple_query("mac set deveui %s", DefaultTimeout, hweui)) {
            return false;
        }
    } else {
        if (!simple_query("mac set deveui %s", DefaultTimeout, otaa.device_eui)) {
            return false;
        }
    }

    // NOTE: This is required anytime you save parameters. Per the RN2903
    // manual, If this parameter was previously saved to user EEPROM by issuing
    // the mac save command, after modifying its value, the mac save command
    // should be called again.
    // https://www.loraserver.io/lora-app-server/use/devices/#to-set-the-appeui-and-appkey
    if (!simple_query("mac set devaddr %s", DefaultTimeout, "00000000")) {
        return false;
    }
    if (!simple_query("mac set nwkskey %s", DefaultTimeout, "00000000000000000000000000000000")) {
        return false;
    }
    if (!simple_query("mac set appskey %s", DefaultTimeout, "00000000000000000000000000000000")) {
        return false;
    }

    if (!save_state()) {
        return false;
    }

    return true;
}

bool Rn2903::provision(const char *app_session_key, const char *network_session_key, const char *device_address, uint32_t uplink_counter,
                       uint32_t downlink_counter) {
    const char *line = nullptr;
    if (!simple_query("sys get hweui", &line, DefaultTimeout)) {
        return false;
    }

    char hweui[64];
    strncpy(hweui, line, sizeof(hweui));

    if (!simple_query("mac set deveui %s", DefaultTimeout, hweui)) {
        return false;
    }
    if (!simple_query("mac set devaddr %s", DefaultTimeout, device_address)) {
        return false;
    }
    if (!simple_query("mac set nwkskey %s", DefaultTimeout, network_session_key)) {
        return false;
    }
    if (!simple_query("mac set appskey %s", DefaultTimeout, app_session_key)) {
        return false;
    }
    if (!simple_query("mac set upctr %d", DefaultTimeout, uplink_counter)) {
        return false;
    }
    if (!simple_query("mac set dnctr %d", DefaultTimeout, downlink_counter)) {
        return false;
    }

    if (!save_state()) {
        return false;
    }

    return true;
}

bool Rn2903::configure_radio(LoraOtaaJoin &otaa) {
    if (otaa.frequency == 915) {
        if (!configure_us915(TTN_US915_DEFAULT_FSB)) {
            return false;
        }
        if (!configure_sf(TTN_US915_DEFAULT_SF)) {
            return false;
        }
    }

    if (otaa.frequency == 868) {
    }

    return true;
}

bool Rn2903::configure_eu868(uint8_t fsb) {
    return true;
}

bool Rn2903::configure_us915(uint8_t fsb) {
    uint8_t ch_low = fsb > 0 ? (fsb - 1) * 8 : 0;
    uint8_t ch_high = fsb > 0 ? ch_low + 7 : 71;
    uint8_t ch_500 = fsb + 63;
    for (uint8_t ch = 0; ch < 72; ch++) {
        if (ch == ch_500 || (ch <= ch_high && ch >= ch_low)) {
            if (!simple_query("mac set ch status %d on", DefaultTimeout, ch)) {
                return false;
            }
            if (ch < 63) {
                if (!simple_query("mac set ch drrange %d 0 3", DefaultTimeout, ch)) {
                    return false;
                }
            }
        } else {
            if (!simple_query("mac set ch status %d off", DefaultTimeout, ch)) {
                return false;
            }
        }
    }
    if (!simple_query("mac set pwridx 5", DefaultTimeout)) {
        return false;
    }
    if (!simple_query("mac set retx 7", DefaultTimeout)) {
        return false;
    }
    return true;
}

bool Rn2903::configure_sf(uint8_t sf) {
    // NOTE This needs to work with other frequencies.
    auto dr = 10 - sf;
    if (!simple_query("mac set dr %d", DefaultTimeout, dr)) {
        return false;
    }

    return true;
}

bool Rn2903::join(LoraOtaaJoin &otaa, int32_t retries, uint32_t retry_delay) {
    if (!provision(otaa)) {
        return false;
    }

    if (!configure_radio(otaa)) {
        return false;
    }

    int8_t tries = 0;
    while (retries == -1 || tries <= retries) {
        if (tries > 0) {
            logwarn("join failed, waiting...");
            fk_delay(retry_delay);
        }

        tries++;

        if (join("otaa")) {
            const char *line = nullptr;
            if (!simple_query("mac save", &line, DefaultTimeout)) {
                return false;
            }

            return true;
        }
    }

    return false;
}

static bool is_string_all_zeros(const char *str) {
    for (auto p = str; *p != 0; ++p) {
        if (*p != '0') {
            return false;
        }
    }
    return true;
}

bool Rn2903::join(const char *mode) {
    if (!simple_query("mac join %s", DefaultTimeout, mode)) {
        return false;
    }

    auto started = fk_uptime();

    const char *line = nullptr;
    for (auto i = 0u; i < 60; ++i) {
        if (read_line_sync(&line, DefaultTimeout, true)) {
            break;
        }
    }
    if (line == nullptr) {
        return false;
    }

    loginfo("rn2903 > '%s' (%d)", line, fk_uptime() - started);

    if (strstr(line, "accepted") == nullptr) {
        return false;
    }

    if (!simple_query("mac get status", &line, DefaultTimeout)) {
        return false;
    }

    if (!simple_query("mac get devaddr", &line, DefaultTimeout)) {
        return false;
    }

    if (is_string_all_zeros(line)) {
        return false;
    }

    return true;
}

bool Rn2903::send_bytes(uint8_t const *data, size_t size, uint8_t port, bool confirmed) {
    char hex[size * 2 + 1];
    bytes_to_hex_string(hex, sizeof(hex), data, size);

    const char *line = nullptr;
    const char *mode = confirmed ? "cnf" : "uncnf";
    if (!simple_query("mac tx %s %d %s", DefaultTimeout, mode, port, hex)) {
        return false;
    }

    auto started = fk_uptime();

    if (!read_line_sync(&line, 60000)) {
        return false;
    }

    loginfo("rn2903 > '%s' (%" PRIu32 "ms)", line, fk_uptime() - started);

    error_ = translate_error(line);

    const char *mac_tx_ok = "mac_tx_ok";
    if (strncmp(line, mac_tx_ok, strlen(mac_tx_ok)) == 0) {
        return true;
    }

    const char *mac_rx = "mac_rx";
    if (strncmp(line, mac_rx, strlen(mac_rx)) == 0) {
        if (!simple_query("mac save", &line, DefaultTimeout)) {
            logwarn("saving on mac_rx");
        }
    }

    return true;
}

constexpr const char *NotJoined = "not_joined";
constexpr const char *MacErr = "mac_err";
constexpr const char *InvalidDataLen = "invalid_data_len";
constexpr const char *KeysNotInit = "keys_not_init";

LoraErrorCode Rn2903::translate_error(const char *line) {
    if (strcmp(line, NotJoined) == 0) {
        return LoraErrorCode::NotJoined;
    }
    if (strcmp(line, MacErr) == 0) {
        return LoraErrorCode::Mac;
    }
    if (strcmp(line, InvalidDataLen) == 0) {
        return LoraErrorCode::DataLength;
    }
    if (strcmp(line, KeysNotInit) == 0) {
        return LoraErrorCode::KeysNotInitialized;
    }
    return LoraErrorCode::None;
}

} // namespace fk

#endif
