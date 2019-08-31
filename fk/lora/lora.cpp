#include <tiny_printf.h>
#include <algorithm>

#include "lora/lora.h"
#include "hal/board.h"
#include "platform.h"
#include "utilities.h"
#include "sc16is740.h"
#include "writer.h"

#if defined(ARDUINO)

#include <Arduino.h>

namespace fk {

FK_DECLARE_LOGGER("lora");

enum class LoraState {
    Initializing,
    Waiting,
};

LoraModule::LoraModule() {
}

class SC16IS740Reader : public Readable {
private:
    SC16IS740 *bridge_;

public:
    SC16IS740Reader(SC16IS740 &bridge);

public:
    int32_t read(uint8_t *buffer, size_t size) override;

};

SC16IS740Reader::SC16IS740Reader(SC16IS740 &bridge) : bridge_(&bridge) {
}

int32_t SC16IS740Reader::read(uint8_t *buffer, size_t size) {
    auto nread = bridge_->available_for_read();
    if (nread > 0) {
        auto can_read = std::min<size_t>(size, nread);
        if (!bridge_->read_fifo(buffer, can_read)) {
            logerror("error reading fifo");
            return -1;
        }
        return can_read;
    }

    return 0;
}

template<size_t N>
class LineReader {
private:
    Readable *reader_;
    char buffer_[N];
    size_t buffer_size_{ N };
    size_t position_{ 0 };
    size_t cursor_{ 0 };
    size_t line_{ 0 };

public:
    LineReader(Readable *reader) : reader_(reader) {
    }

public:
    bool read_line_sync(const char **line, uint32_t to) {
        auto deadline = fk_uptime() + to;

        while (fk_uptime() < deadline) {
            if (read_line_async(line)) {
                return true;
            }

            fk_delay(10);
        }

        return false;
    }

    bool read_line_async(const char **line) {
        (*line) = nullptr;

        auto available = (size_t)(buffer_size_ - position_);
        if (available > 0) {
            auto nread = reader_->read((uint8_t *)(buffer_ + position_), available);
            if (nread < 0) {
                return nread;
            }
            if (nread > 0) {
                position_ += nread;
            }
        }

        for (size_t i = cursor_; i < position_; ++i) {
            auto c = buffer_[i];
            if (c == '\n' || c == '\r') {
                buffer_[i] = 0;
                *line = buffer_ + line_;
                cursor_ = i + 1;
                line_ = i + 1;
                if (cursor_ == position_) {
                    cursor_ = 0;
                    position_ = 0;
                    line_ = 0;
                }
                if (strlen(*line) > 0) {
                    return true;
                }
            }
        }

        return false;
    }

};

class Rn2903 {
private:
    SC16IS740 bridge_;
    SC16IS740Reader reader_{ bridge_ };
    LineReader<256> line_reader_{ &reader_ };

public:
    Rn2903(TwoWireWrapper &bud);

public:
    bool begin() {
        return bridge_.begin();
    }

    bool read_line_sync(const char **line, uint32_t to) {
        if (!line_reader_.read_line_sync(line, to)) {
            logerror("error reading line");
            return false;
        }

        loginfo("rn2903 > '%s'", *line);

        return true;
    }

    bool simple_query(const char *cmd, const char **line, uint32_t to, ...) {
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

    bool save_state() {
        const char *line = nullptr;
        if (!simple_query("mac save", &line, 1000)) {
            return false;
        }

        return true;
    }

    bool provision(const char *app_eui, const char *app_key) {
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

    bool configure_us915(uint8_t fsb) {
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

    #define TTN_DEFAULT_SF  7
    #define TTN_DEFAULT_FSB 2

    bool configure_sf(uint8_t sf) {
        const char *line = nullptr;

        // NOTE This needs to work with other frequencies.
        auto dr = 10 - sf;
        if (!simple_query("mac set dr %d", &line, 1000, dr)) {
            return false;
        }

        return true;
    }

    bool join(const char *app_eui, const char *app_key) {
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
                delay(retry_delay);
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

};

Rn2903::Rn2903(TwoWireWrapper &bus) : bridge_{ bus } {
}

bool LoraModule::begin() {
    pinMode(LORA_ENABLE, OUTPUT);
    digitalWrite(LORA_ENABLE, LOW);
    fk_delay(500);
    digitalWrite(LORA_ENABLE, HIGH);
    fk_delay(100);

    auto bus = get_board()->i2c_radio();

    Rn2903 rn2903{ bus };
    if (!rn2903.begin()) {
        return false;
    }

    const char *line = nullptr;
    if (!rn2903.read_line_sync(&line, 5000)) {
        return false;
    }

    const char *dev_eui = "008C1F21E6162E58";
    if (!rn2903.simple_query("mac set deveui %s", &line, 1000, dev_eui)) {
        return false;
    }

    if (!rn2903.simple_query("sys get hweui", &line, 1000)) {
        return false;
    }
    if (!rn2903.simple_query("sys get vdd", &line, 1000)) {
        return false;
    }
    if (!rn2903.simple_query("mac get appeui", &line, 1000)) {
        return false;
    }
    if (!rn2903.simple_query("mac get deveui", &line, 1000)) {
        return false;
    }
    if (!rn2903.simple_query("mac get dr", &line, 1000)) {
        return false;
    }
    if (!rn2903.simple_query("mac get rxdelay1", &line, 1000)) {
        return false;
    }
    if (!rn2903.simple_query("mac get rxdelay2", &line, 1000)) {
        return false;
    }

    const char *app_eui = "70B3D57ED001F439";
    const char *app_key = "900AA739F7AA08D083CDE23E96FA0820";

    while (true) {
        if (rn2903.join(app_eui, app_key)) {
            break;
        }

        fk_delay(5000);
    }

    return true;
}

}

#endif
