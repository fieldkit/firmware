#include <tiny_printf.h>

#include "common.h"
#include "hal/board.h"
#include "hal/metal/metal_sd_card.h"
#include "platform.h"
#include "format_sd_card.h"
#include "standard_page.h"

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

    auto number_of_blocks = sd_.card()->cardSize();
    if (number_of_blocks == 0) {
        logwarn("invalid size");
        return false;
    }

    auto capacity_mb = (number_of_blocks + 2047) / 2048;

    loginfo("card capacity: %" PRIu32 "MB blocks: %" PRIu32, capacity_mb, number_of_blocks);

    return true;
}

bool MetalSdCard::append_logs(circular_buffer<char> &buffer) {
    return append_logs(buffer, buffer.begin());
}

bool MetalSdCard::append_logs(circular_buffer<char> &buffer, circular_buffer<char>::iterator iter) {
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

    if (log_initialized_) {
        SdFile file;

        if (!file.open(log_file_name_, O_WRONLY | O_CREAT | O_APPEND)) {
            logerror("error opening %s", log_file_name_);
            return false;
        }

        auto size = 0u;
        StandardPage page;
        Buffer writing{ (uint8_t *)page.ptr(), page.size() };
        for ( ; iter != buffer.end(); ++iter) {
            writing.write(*iter);
            if (writing.full()) {
                file.write(writing.ptr(), writing.position());
                writing.clear();
            }
            size++;
        }

        if (writing.position() > 0) {
            file.write(writing.ptr(), writing.position());
        }
        file.flush();
        file.close();

        loginfo("flushed %d to %s (%" PRIu32 "ms) (%" PRIu32 " bytes)", size, log_file_name_, fk_uptime() - started, file.fileSize());
    }
    else {
        loginfo("ignored (%" PRIu32 "ms)", fk_uptime() - started);
        log_initialized_ = false;
    }

    return true;
}

bool MetalSdCard::is_file(const char *path) {
    if (!sd_.exists(path)) {
        return false;
    }

    SdFile file;
    if (!file.open(path)) {
        return false;
    }
    return file.isFile();
}

bool MetalSdCard::is_directory(const char *path) {
    if (!sd_.exists(path)) {
        return false;
    }

    SdFile file;
    if (!file.open(path)) {
        return false;
    }
    return file.isDir();
}

bool MetalSdCard::mkdir(const char *path) {
    if (!sd_.mkdir(path)) {
        return false;
    }
    return true;
}

bool MetalSdCard::unlink(const char *path) {
    return sd_.remove(path);
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

SdCardFile *MetalSdCard::open(const char *path, bool writing, Pool &pool) {
    loginfo("opening '%s'", path);

    auto flags = writing ? O_WRONLY | O_CREAT | O_EXCL : O_RDONLY;

    return new (pool) MetalSdCardFile(path, flags);
}

MetalSdCardFile::MetalSdCardFile(const char *path, oflag_t oflag) : file_(path, oflag) {
}

int32_t MetalSdCardFile::write(uint8_t const *buffer, size_t size) {
    return file_.write(buffer, size);
}

int32_t MetalSdCardFile::read(uint8_t *buffer, size_t size) {
    return file_.read(buffer, size);
}

int32_t MetalSdCardFile::seek_beginning() {
    file_.seekSet(0);
    return 0;
}

int32_t MetalSdCardFile::seek_end() {
    file_.seekEnd(0);
    return 0;
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
