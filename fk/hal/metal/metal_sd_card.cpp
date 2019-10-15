#include <tiny_printf.h>

#include "common.h"
#include "hal/board.h"
#include "hal/metal/metal_sd_card.h"
#include "platform.h"
#include "format_sd_card.h"

#if defined(__SAMD51__)

namespace fk {

FK_DECLARE_LOGGER("sdcard");

MetalSdCard::MetalSdCard() : sd_(&SD_SPI) {
}

bool MetalSdCard::begin() {
    SD_SPI.end();

    SD_SPI.begin();

    if (!sd_.begin(PIN_SD_CS, SD_SCK_MHZ(50))) {
        return false;
    }

    if (sd_.vol()->fatType() == 0) {
        logwarn("invalid FAT system");
        return false;
    }

    auto size = sd_.card()->cardSize();
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
        if (sd_.card()->errorCode()) {
            loginfo("ignoring logs: no sd card, error = 0x%x", sd_.card()->errorCode());
        }
        return false;
    }
    else {
        if (!log_initialized_) {
            FormattedTime formatted{ get_clock_now(), TimeFormatMachine };

            if (!sd_.mkdir(formatted.cstr())) {
                logerror("error making directory '%s'", formatted.cstr());
                return false;
            }

            for (auto counter = 0; counter < 1000; ++counter) {
                tiny_snprintf(log_file_name_, sizeof(log_file_name_), "/%s/fkl_%03d.txt", formatted.cstr(), counter);
                if (!sd_.exists(log_file_name_)) {
                    loginfo("picked file name %s", log_file_name_);
                    log_initialized_ = true;
                    break;
                }
            }
        }
    }

    auto size = buffer.size();
    if (log_initialized_) {
        SdFile file;

        if (!file.open(log_file_name_, O_WRONLY | O_CREAT | O_APPEND)) {
            logerror("error opening %s", log_file_name_);
            return false;
        }

        file.write(buffer.buffer(), size);
        file.flush();
        file.close();

        loginfo("flushed %d to %s (%" PRIu32 "ms) (%" PRIu32 " bytes)", size, log_file_name_, fk_uptime() - started, file.fileSize());
    }
    else {
        loginfo("ignored %d (%" PRIu32 "ms)", size, fk_uptime() - started);
        log_initialized_ = false;
    }

    return true;
}

bool MetalSdCard::is_file(const char *path) {
    if (!sd_.exists(path)) {
        return false;
    }

    return false; // sd.isFile(name);
}

bool MetalSdCard::is_directory(const char *path) {
    if (!sd_.exists(path)) {
        return false;
    }

    return false; // sd.isDirectory(name);
}

bool MetalSdCard::mkdir(const char *path) {
    if (!sd_.mkdir(path)) {
        return false;
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

SdCardFile *MetalSdCard::open(const char *path, Pool &pool) {
    if (sd_.exists(path)) {
        loginfo("file '%s' exists, removing", path);

        if (!sd_.remove(path)) {
            logerror("unable to remove '%s'", path);
            return nullptr;
        }
    }

    loginfo("opening '%s'", path);

    return new (pool) MetalSdCardFile(path, O_WRONLY | O_CREAT | O_EXCL);
}

MetalSdCardFile::MetalSdCardFile(const char *path, oflag_t oflag) : file_(path, oflag) {
}

int32_t MetalSdCardFile::write(uint8_t const *buffer, size_t size) {
    return file_.write(buffer, size);
}

size_t MetalSdCardFile::file_size() {
    return file_.fileSize();
}

bool MetalSdCardFile::close() {
    file_.flush();

    return file_.close();
}

bool MetalSdCardFile::is_open() const {
    return file_.isFile() && file_.isOpen();
}

}

#endif
