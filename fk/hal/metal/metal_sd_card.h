#pragma once

#include "config.h"
#include "hal/clock.h"
#include "hal/sd_card.h"

#if defined(__SAMD51__)

#include <SdFat.h>
#include <sdios.h>

namespace fk {

class MetalSdCard : public SdCard {
public:
    constexpr static size_t MaximumPathLength = 128;
    constexpr static size_t MaximumDirectoryNameLength = 32;

private:
    SdFat sd_;
    Availability availability_{ Availability::Unknown };
    uint32_t log_time_{ 0 };
    char log_file_name_[MaximumPathLength] = { 0 };
    char name_[MaximumDirectoryNameLength] = { 0 };
    bool log_initialized_{ false };
    uint32_t log_writes_{ 0 };

public:
    MetalSdCard();

public:
    bool begin() override;
    bool append_logs(circular_buffer<char> &buffer) override;
    bool append_logs(circular_buffer<char> &buffer, circular_buffer<char>::iterator iter) override;
    bool append_logs(uint8_t const *buffer, size_t size) override;
    bool close_logs() override;
    bool is_file(const char *path) override;
    bool is_directory(const char *path) override;
    bool mkdir(const char *path) override;
    bool unlink(const char *path) override;
    SdCardFile *open(const char *path, OpenFlags flags, Pool &pool) override;
    bool format() override;
    bool ls(const char *path, size_t skip, fk_app_DirectoryEntry **files, size_t &number_entries, size_t &total_entries,
            Pool &pool) override;
    void name(const char *name) override;

private:
    bool initialize_logs();
};

class MetalSdCardFile : public SdCardFile {
private:
    SdFile file_;

public:
    MetalSdCardFile(const char *path, oflag_t oflag);

public:
    int32_t write(uint8_t const *buffer, size_t size) override;
    int32_t read(uint8_t *buffer, size_t size) override;
    int32_t seek_beginning() override;
    int32_t seek_end() override;
    int32_t seek_from_end(int32_t offset) override;
    size_t file_size() override;
    bool is_open() const override;
    bool close() override;
};

} // namespace fk

#endif
