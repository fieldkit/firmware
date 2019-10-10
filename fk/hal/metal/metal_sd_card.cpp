#include <tiny_printf.h>

#include "common.h"
#include "hal/board.h"
#include "hal/metal/metal_sd_card.h"
#include "platform.h"

#if defined(__SAMD51__)

#include <SPI.h>

namespace fk {

#define SPI_SPEED SD_SCK_MHZ(50)
#define LOG_FILE_BASE_NAME "fkl_"

FK_DECLARE_LOGGER("sdcard");

static SdFat sd(&SD_SPI);
static SdFile log_file;
static char log_file_name[13] = LOG_FILE_BASE_NAME "000.txt";
static bool log_initialized{ false };
static bool log_ready{ false };

MetalSdCard::MetalSdCard() {
}

bool MetalSdCard::begin() {
    SD_SPI.begin();

    if (!sd.begin(PIN_SD_CS, SPI_SPEED)) {
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
    auto started = fk_uptime();

    if (!begin()) {
        log_initialized = false;
        log_ready = false;
        if (sd.card()->errorCode()) {
            loginfo("ignoring logs: no sd card, error = 0x%x", sd.card()->errorCode());
        }
        return false;
    }

    if (!log_initialized) {
        log_initialized = true;

        for (auto counter = 0; counter < 1000; ++counter) {
            tiny_snprintf(log_file_name, sizeof(log_file_name), "%s%03d.txt", LOG_FILE_BASE_NAME, counter);
            if (!sd.exists(log_file_name)) {
                break;
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
        loginfo("flushed %d to %s (%" PRIu32 "ms) (%" PRIu32 " bytes)", size, log_file_name, fk_uptime() - started, log_file.fileSize());
    }
    else {
        loginfo("ignored %d to (%" PRIu32 "ms)", size, fk_uptime() - started);
    }

    return true;
}

}

#endif
