#include <tiny_printf.h>

#include "rn2903.h"

#if defined(ARDUINO)

namespace fk {

FK_DECLARE_LOGGER("lora");

#define TTN_DEFAULT_SF  7
#define TTN_DEFAULT_FSB 2

Rn2903::Rn2903(TwoWireWrapper &bus) : bridge_{ bus } {
}

bool Rn2903::begin() {
    return bridge_.begin();
}

bool Rn2903::read_line_sync(const char **line, uint32_t to) {
    if (!line_reader_.read_line_sync(line, to)) {
        logerror("error reading line");
        return false;
    }

    loginfo("rn2903 > '%s'", *line);

    return true;
}

bool Rn2903::simple_query(const char *cmd, const char **line, uint32_t to, ...) {
    va_list args;

    va_start(args, to);
    char buffer[256];
    auto needed = tiny_vsnprintf(buffer, sizeof(buffer) - 3, cmd, args);
    FK_ASSERT(needed + 3 < (int32_t)sizeof(buffer));
    va_end(args);

    loginfo("rn2903 < '%s'", buffer);

    buffer[needed + 0] = '\r';
    buffer[needed + 1] = '\n';
    buffer[needed + 2] = 0;

    if (!bridge_.write(buffer)) {
        logerror("error writing line");
        return false;
    }

    if (!read_line_sync(line, to)) {
        return false;
    }

    return true;
}

bool Rn2903::save_state() {
    const char *line = nullptr;
    if (!simple_query("mac save", &line, 1000)) {
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

    if (!simple_query("mac set deveui %s", &line, 1000, hweui)) {
        return false;
    }

    if (!simple_query("mac set appeui %s", &line, 1000, app_eui)) {
        return false;
    }

    if (!simple_query("mac set appkey %s", &line, 1000, app_key)) {
        return false;
    }

    if (!save_state()) {
        return false;
    }

    return true;
}

bool Rn2903::configure_us915(uint8_t fsb) {
    const char *line = nullptr;
    uint8_t ch_low = fsb > 0 ? (fsb - 1) * 8 : 0;
    uint8_t ch_high = fsb > 0 ? ch_low + 7 : 71;
    uint8_t ch_500 = fsb + 63;
    for (uint8_t ch = 0; ch < 72; ch++) {
        if (ch == ch_500 || (ch <= ch_high && ch >= ch_low)) {
            if (!simple_query("mac set ch status %d on", &line, 1000, ch)) {
                return false;
            }
            if (ch < 63) {
                if (!simple_query("mac set ch drrange %d 0 3", &line, 1000, ch)) {
                    return false;
                }
            }
        }
        else {
            if (!simple_query("mac set ch status %d off", &line, 1000, ch)) {
                return false;
            }
        }
    }
    if (!simple_query("mac set pwridx 5", &line, 1000)) {
        return false;
    }
    if (!simple_query("mac set retx 7", &line, 1000)) {
        return false;
    }
    return true;
}

bool Rn2903::configure_sf(uint8_t sf) {
    const char *line = nullptr;

    // NOTE This needs to work with other frequencies.
    auto dr = 10 - sf;
    if (!simple_query("mac set dr %d", &line, 1000, dr)) {
        return false;
    }

    return true;
}

bool Rn2903::join(const char *app_eui, const char *app_key) {
    if (!provision(app_eui, app_key)) {
        return false;
    }

    if (!configure_us915(TTN_DEFAULT_FSB)) {
        return false;
    }

    if (!configure_sf(TTN_DEFAULT_SF)) {
        return false;
    }

    int8_t retries = 3;
    int8_t tries = 0;
    uint32_t retry_delay = 10000;

    while (retries == -1 || tries <= retries) {
        tries++;

        const char *line = nullptr;

        if (!simple_query("mac join otaa", &line, 1000)) {
            logwarn("join failed, waiting...");
            fk_delay(retry_delay);
            continue;
        }

        if (read_line_sync(&line, 10000)) {
            return false;
        }

        if (strstr(line, "accepted") == nullptr) {
            return false;
        }

        if (!simple_query("mac get ch status", &line, 1000)) {
            return false;
        }

        if (!simple_query("mac get devaddr", &line, 1000)) {
            return false;
        }

        return true;
    }

    return false;
}

}

#endif
