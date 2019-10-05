// #include <phylum/backend.h>
// #include <backends/arduino_sd/arduino_sd.h>

#include "common.h"
#include "hal/board.h"
#include "hal/metal/metal_sd_card.h"
#include "platform.h"

#if defined(__SAMD51__)

#include <SPI.h>

namespace fk {

#define SPI_SPEED SD_SCK_MHZ(50)
#define LOG_FILE_BASE_NAME "fklogs"

FK_DECLARE_LOGGER("sdcard");

static SdFat sd(&SD_SPI);
static SdFile log_file;
static char log_file_name[13] = LOG_FILE_BASE_NAME "00.txt";
static bool log_initialized{ false };
static bool log_ready{ false };

MetalSdCard::MetalSdCard() {
}

bool MetalSdCard::begin() {
    SD_SPI.begin();

    if (!sd.begin(PIN_SD_CS, SPI_SPEED)) {
        if (sd.card()->errorCode()) {
            logwarn("sd card error: 0x%x", sd.card()->errorCode());
        }
        return false;
    }

    if (sd.vol()->fatType() == 0) {
        logwarn("invalid FAT system");
        return false;
    }

    auto size = sd.card()->cardSize();
    if (size == 0) {
        logwarn("invalid size");
        return false;
    }

    return true;
}

bool MetalSdCard::append_logs(circular_buffer<char> &buffer) {
    static constexpr uint8_t BaseNameSize = sizeof(LOG_FILE_BASE_NAME) - 1;

    auto started = fk_uptime();

    if (!begin()) {
        return false;
    }

    if (!log_initialized) {
        log_initialized = true;

        if (BaseNameSize > 6) {
            logerror("log file base name is too long");
            return false;
        }

        while (sd.exists(log_file_name)) {
            if (log_file_name[BaseNameSize + 1] != '9') {
                log_file_name[BaseNameSize + 1]++;
            }
            else if (log_file_name[BaseNameSize] != '9') {
                log_file_name[BaseNameSize + 1] = '0';
                log_file_name[BaseNameSize]++;
            }
            else {
                logerror("error creating file name");
                return false;
            }
        }

        if (!log_file.open(log_file_name, O_WRONLY | O_CREAT | O_EXCL)) {
            logerror("error opening %s", log_file_name);
            return false;
        }

        loginfo("opened %s", log_file_name);
        log_ready = true;
    }

    auto size = buffer.size();

    if (log_ready) {
        log_file.write(buffer.buffer(), size);
        log_file.flush();
        loginfo("flushed %d to %s (%" PRIu32 "ms)", size, log_file_name, fk_uptime() - started);
    }
    else {
        loginfo("ignored %d to %s (%" PRIu32 "ms)", size, log_file_name, fk_uptime() - started);
    }

    buffer.clear();

    return true;
}

}

#endif
