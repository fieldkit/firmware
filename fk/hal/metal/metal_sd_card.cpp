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
#undef max

#if defined(__SAMD51__)

namespace fk {

FK_DECLARE_LOGGER("sdcard");

constexpr size_t SdMaximumNameLength = 30;

static optional<bool> open_path(SdFile &dir, const char *path, oflag_t oflag);

static bool number_of_files(SdFile &dir, size_t &nfiles);

MetalSdCard::MetalSdCard() : sd_(&SD_SPI) {
    strncpy(name_, "Boot", sizeof(name_));
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

void MetalSdCard::name(const char *name) {
    if (strncmp(name_, name, sizeof(name_)) == 0) {
        return;
    }
    strncpy(name_, name, sizeof(name_));
    log_initialized_ = false;
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
            FormattedTime formatted{ log_time_, TimeFormatLogs };

            tiny_snprintf(log_file_name_, sizeof(log_file_name_), "/%s", name_);
            if (!sd_.exists(log_file_name_)) {
                if (!sd_.mkdir(log_file_name_)) {
                    logerror("error making directory '%s'", log_file_name_);
                    return false;
                }
            }

            for (auto counter = 0u; counter < 100u; ++counter) {
                tiny_snprintf(log_file_name_, sizeof(log_file_name_), "/%s/%s_%02d.txt", name_, formatted.cstr(), counter);
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

bool MetalSdCard::ls(const char *path, size_t skip, fk_app_DirectoryEntry** dir_entries, size_t &number_entries, size_t &total_entries, Pool &pool) {
    FK_ASSERT_ADDRESS(dir_entries);

    *dir_entries = nullptr;
    number_entries = 0;
    total_entries = 0;

    SdFile dir;
    auto opened = open_path(dir, path, O_RDONLY);
    if (!opened) {
        return false;
    }

    if (!*opened) {
        logwarn("no such path, todo-404");
        return false;
    }

    if (!number_of_files(dir, total_entries)) {
        return false;
    }

    loginfo("ls total-entries=%zu", total_entries);

    if (total_entries == 0) {
        return true;
    }

    // Avoid overflowing page allocation by paging.
    auto max_files_per_ls = StandardPageSize / sizeof(fk_app_DirectoryEntry) / 2;
    auto returning_entries = (size_t)std::min((int32_t)max_files_per_ls, std::max((int32_t)(total_entries - skip), (int32_t)0));
    loginfo("ls netries=%zu max=%zu skip=%zu", returning_entries, max_files_per_ls, skip);
    if (returning_entries == 0) {
        return true;
    }

    auto entries = (fk_app_DirectoryEntry *)pool.malloc(sizeof(fk_app_DirectoryEntry) * returning_entries);

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

            logdebug("[%zu/%zu] file '%s'", index, file_number, name);

            index++;
        }
        file_number++;
        file.close();

        if (index == returning_entries) {
            break;
        }
    }

    dir.close();

    *dir_entries = entries;
    number_entries = returning_entries;

    return true;
}

SdCardFile *MetalSdCard::open(const char *path, OpenFlags flags, Pool &pool) {
    auto sd_flags = 0;
    // These are all or can be considered mutually exclusive.
    if (flags == OpenFlags::Read) {
        sd_flags = O_RDONLY;
    }
    else if (flags == OpenFlags::Append) {
        sd_flags = O_WRONLY | O_CREAT | O_EXCL | O_APPEND;
    }
    else if (flags == OpenFlags::Write) {
        sd_flags = O_WRONLY | O_CREAT | O_EXCL;
    }
    loginfo("opening '%s' (%d)", path, sd_flags);
    return new (pool) MetalSdCardFile(path, sd_flags);
}

MetalSdCardFile::MetalSdCardFile(const char *path, oflag_t oflag) {
    if (path[0] == '/') {
        auto opened = open_path(file_, path, oflag);
        if (!opened) {
            logerror("open failed (serious)");
        }
        else if (!*opened) {
            logerror("open failed");
        }
    }
    else {
        if (!file_.open(path, oflag)) {
            logwarn("open failed");
        }
    }
    if (!file_.isFile())  {
        logwarn("open failed, no file");
    }
    if (!file_.isOpen())  {
        logwarn("open failed, closed");
    }
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

/**
 * I wish there was an easier way to do this. This starts at the root
 * and opens up an arbitrary directory so we can do the listing.
 */
static optional<bool> open_path(SdFile &dir, const char *path, oflag_t oflag) {
    auto p = path;

    if (p[0] != '/') {
        logerror("absoulte path required");
        return nullopt;
    }

    if (oflag & O_CREAT) {
        loginfo("creating new file: %s", path);
        return dir.open(path, oflag);
    }

    p++;

    loginfo("opening: /%s", *p == 0 ? " E" : "");
    if (!dir.open("/")) {
        logerror("unable to open %s", path);
        return nullopt;
    }

    while (*p != 0) {
        char name[SdMaximumNameLength];

        auto slash = strchr(p, '/');
        size_t length = slash == nullptr ? strlen(p) : slash - p;
        auto end = slash == nullptr;

        bzero(name, sizeof(name));
        strncpy(name, p, std::min(length, SdMaximumNameLength));

        loginfo("opening: %s%s", name, end ? " E" : "");

        dir.rewind();

        auto found = false;
        SdFile file;
        while (file.openNext(&dir, end ? oflag : O_RDONLY)) {
            file.getName(name, sizeof(name));
            if (strncmp(name, p, length) == 0) {
                loginfo("path found");
                found = true;
                dir = file;
                break;
            }
            file.close();
        }

        if (!found) {
            loginfo("path missing");
            return false;
        }

        if (slash == nullptr) {
            loginfo("end of path");
            break;
        }

        p = slash + 1;
    }

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

}

#endif
