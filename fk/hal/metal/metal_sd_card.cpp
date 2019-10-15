#include <tiny_printf.h>

#include "common.h"
#include "hal/board.h"
#include "hal/metal/metal_sd_card.h"
#include "platform.h"
#include "format_sd_card.h"

#if defined(__SAMD51__)

#include <SPI.h>

namespace fk {

FK_DECLARE_LOGGER("sdcard");

#define LOG_FILE_BASE_NAME "fkl_"

static SdFat sd(&SD_SPI);
static SdFile log_file;
static char log_file_name[13] = LOG_FILE_BASE_NAME "000.txt";
static bool log_initialized{ false };

MetalSdCard::MetalSdCard() {
}

bool MetalSdCard::begin() {
    SD_SPI.end();

    SD_SPI.begin();

    if (!sd.begin(PIN_SD_CS, SD_SCK_MHZ(50))) {
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

    loginfo("card size: %" PRIu32, size);

    return true;
}

bool MetalSdCard::append_logs(circular_buffer<char> &buffer) {
    auto started = fk_uptime();

    if (!begin()) {
        if (sd.card()->errorCode()) {
            loginfo("ignoring logs: no sd card, error = 0x%x", sd.card()->errorCode());
        }
        return false;
    }
    else {
        if (!log_initialized) {
            for (auto counter = 0; counter < 1000; ++counter) {
                tiny_snprintf(log_file_name, sizeof(log_file_name), "%s%03d.txt", LOG_FILE_BASE_NAME, counter);
                if (!sd.exists(log_file_name)) {
                    loginfo("picked file name %s", log_file_name);
                    log_initialized = true;
                    break;
                }
            }
        }
    }

    auto size = buffer.size();
    if (log_initialized) {
        if (!log_file.open(log_file_name, O_WRONLY | O_CREAT | O_EXCL)) {
            logerror("error opening %s", log_file_name);
            return false;
        }

        log_file.write(buffer.buffer(), size);
        log_file.flush();
        loginfo("flushed %d to %s (%" PRIu32 "ms) (%" PRIu32 " bytes)", size, log_file_name, fk_uptime() - started, log_file.fileSize());
    }
    else {
        loginfo("ignored %d (%" PRIu32 "ms)", size, fk_uptime() - started);
        log_initialized = false;
    }

    return true;
}

bool MetalSdCard::format() {
    loginfo("formatting...");

    FormatSdCard formatter;
    if (!formatter.begin()) {
        logerror("error opening");
        return false;
    }

    if (!formatter.erase()) {
        logerror("error erasing");
        return false;
    }

    if (!formatter.format()) {
        logerror("error formatting");
        return false;
    }

    loginfo("done");

    return true;
}

SdCardFile *MetalSdCard::open(const char *name, Pool &pool) {
    static SdFile file;

    if (sd.exists(name)) {
        if (!sd.remove(name)) {
            return new (pool) MetalSdCardFile();
        }
    }

    if (!file.open(name, O_WRONLY | O_CREAT | O_EXCL)) {
        return new (pool) MetalSdCardFile();
    }

    return new (pool) MetalSdCardFile(file);
}

MetalSdCardFile::MetalSdCardFile() {
}

MetalSdCardFile::MetalSdCardFile(SdFile file) : file_(file) {
}

int32_t MetalSdCardFile::write(uint8_t const *buffer, size_t size) {
    return file_.write(buffer, size);
}

size_t MetalSdCardFile::file_size() {
    return file_.fileSize();
}

bool MetalSdCardFile::close() {
    return file_.close();
}

}

#endif
