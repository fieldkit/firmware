#pragma once

#include "hal/sd_card.h"

namespace fk {

class LinuxSdCard : public SdCard {
public:
    LinuxSdCard();

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

};

class LinuxSdCardFile : public SdCardFile {
public:
    int32_t write(uint8_t const *buffer, size_t size) override;
    int32_t read(uint8_t *buffer, size_t size) override;
    int32_t seek_beginning() override;
    int32_t seek_end() override;
    size_t file_size() override;
    bool close() override;
    bool is_open() const override {
        return false;
    }

};

}
