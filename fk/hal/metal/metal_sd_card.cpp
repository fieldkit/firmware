#include <cmath>

#include <tiny_printf.h>

#include "common.h"
#include "hal/board.h"
#include "hal/metal/metal_sd_card.h"
#include "platform.h"
#include "format_sd_card.h"
#include "standard_page.h"
#include "protobuf.h"

// NOTE This is so embarassing.
#undef abs
#undef min

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

class LogFile {
private:
    MetalSdCard *card_{ nullptr };

public:
    LogFile(MetalSdCard *card);
    virtual ~LogFile();

public:
    bool open();

public:

};

LogFile::LogFile(MetalSdCard *card) : card_(card) {
}

LogFile::~LogFile() {
}

bool LogFile::open() {
    return false;
}

bool MetalSdCard::initialize_logs() {
    if (!begin()) {
        if (sd_.card()->errorCode()) {
            loginfo("ignoring logs: no sd card, error = 0x%x", sd_.card()->errorCode());
        }
        return false;
    }
    else {
        auto now = get_clock_now();
        if (log_time_ == 0 || std::abs(now - log_time_) > OneDayMs) {
            log_time_ = get_clock_now();
            log_initialized_ = false;
        }
        if (!log_initialized_) {
            FormattedTime formatted{ log_time_, TimeFormatMachine };

            if (!sd_.exists(formatted.cstr())) {
                if (!sd_.mkdir(formatted.cstr())) {
                    logerror("error making directory '%s'", formatted.cstr());
                    return false;
                }
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

    return true;
}

bool MetalSdCard::append_logs(circular_buffer<char> &buffer, circular_buffer<char>::iterator iter) {
    auto started = fk_uptime();

    initialize_logs();

    if (log_initialized_) {
        SdFile file;

        if (!file.open(log_file_name_, O_WRONLY | O_CREAT | O_APPEND)) {
            logerror("error opening %s", log_file_name_);
            return false;
        }

        auto size = 0u;
        StandardPage page{ __func__ };
        Buffer writing{ (uint8_t *)page.ptr(), page.size() };
        for ( ; iter != buffer.end(); ++iter) {
            if (*iter != 0) {
                writing.write(*iter);
                if (writing.full()) {
                    file.write(writing.ptr(), writing.position());
                    writing.clear();
                }
                size++;
            }
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
    }

    return true;
}

bool MetalSdCard::append_logs(uint8_t const *buffer, size_t size) {
    auto started = fk_uptime();

    initialize_logs();

    if (log_initialized_) {
        SdFile file;

        if (!file.open(log_file_name_, O_WRONLY | O_CREAT | O_APPEND)) {
            logerror("error opening %s", log_file_name_);
            return false;
        }

        file.write(buffer, size);

        file.flush();
        file.close();

        loginfo("flushed %d to %s (%" PRIu32 "ms) (%" PRIu32 " bytes)", size, log_file_name_, fk_uptime() - started, file.fileSize());
    }
    else {
        loginfo("ignored (%" PRIu32 "ms)", fk_uptime() - started);
    }

    return true;
}

bool MetalSdCard::close_logs() {
    log_initialized_ = false;
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

static bool number_of_files(SdFile &dir, size_t &nfiles) {
    dir.rewind();
    nfiles = 0;

    SdFile file;
    while (file.openNext(&dir, O_RDONLY)) {
        nfiles++;
        file.close();
    }

    return true;
}

constexpr size_t SdMaximumNameLength = 13;

/**
 * I wish there was an easier way to do this. This starts at the root
 * and opens up an arbitrary directory so we can do the listing.
 */
static bool open_path(SdFat &sd, SdFile &dir, const char *path, Pool &pool) {
    auto copy = pool.strdup(path);
    auto p = copy;

    if (p[0] != '/') {
        logerror("absoulte path required");
        return false;
    }

    loginfo("opening: /");
    if (!dir.open("/")) {
        logerror("unable to open %s", path);
        return false;
    }

    p++;

    while (*p != 0) {
        auto slash = strchr(p, '/');
        if (slash != nullptr) {
            *slash = 0;
        }

        loginfo("opening: %s", p);

        dir.rewind();

        SdFile file;
        while (file.openNext(&dir, O_RDONLY)) {
            char name[SdMaximumNameLength];
            file.getName(name, sizeof(name));
            if (strcmp(name, p) == 0) {
                dir = file;
                break;
            }
            file.close();
        }

        if (slash == nullptr) {
            break;
        }

        p = slash + 1;
    }

    return true;
}

bool MetalSdCard::ls(const char *path, size_t skip, fk_app_DirectoryEntry** files, size_t &nfiles, Pool &pool) {
    FK_ASSERT_ADDRESS(files);

    *files = nullptr;
    nfiles = 0;

    SdFile dir;
    if (!open_path(sd_, dir, path, pool)) {
        return false;
    }

    auto files_in_directory = 0u;
    if (!number_of_files(dir, files_in_directory)) {
        return false;
    }

    loginfo("ls nentries-dir=%zu", files_in_directory);

    if (files_in_directory == 0) {
        return true;
    }

    // Avoid overflowing page allocation by paging.
    auto max_files_per_ls = StandardPageSize / sizeof(fk_app_DirectoryEntry) / 2;
    auto number_entries = std::min(max_files_per_ls, files_in_directory - skip);
    auto entries = (fk_app_DirectoryEntry *)pool.malloc(sizeof(fk_app_DirectoryEntry) * number_entries);

    loginfo("ls netries=%zu max=%zu skip=%zu", number_entries, max_files_per_ls, skip);

    auto file_number = 0u;
    auto index = 0u;

    dir.rewind();

    SdFile file;
    while (file.openNext(&dir, O_RDONLY)) {
        if (file_number >= skip) {
            auto name = (char *)pool.malloc(SdMaximumNameLength);
            if (!file.getName(name, SdMaximumNameLength)) {
                logerror("unable to get file name");
                return false;
            }

            entries[index] = fk_app_DirectoryEntry_init_default;
            entries[index].directory = file.isDir();
            entries[index].size = file.fileSize();
            entries[index].name.arg = (void *)name;
            entries[index].name.funcs.encode = pb_encode_string;

            loginfo("[%zu/%zu] file '%s'", index, file_number, name);

            index++;
        }
        file_number++;
        file.close();

        if (index == number_entries) {
            break;
        }
    }

    dir.close();

    *files = entries;
    nfiles = number_entries;

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
