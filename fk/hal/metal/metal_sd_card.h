#pragma once

#include "hal/sd_card.h"
#include "clock.h"

#if defined(__SAMD51__)

#include <SdFat.h>
#include <sdios.h>

namespace fk {

class MetalSdCard : public SdCard {
private:
    SdFat sd_;
    uint32_t log_time_{ 0 };
    char log_file_name_[1 + MaximumLengthOfTimeString + 1 + 13];
    bool log_initialized_{ false };

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
    SdCardFile *open(const char *path, bool writing, Pool &pool) override;
    bool format() override;
    bool ls(const char *path, size_t skip, fk_app_DirectoryEntry **files, size_t &nfiles, Pool &pool) override;

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
    size_t file_size() override;
    bool is_open() const override;
    bool close() override;

};

}

#endif
