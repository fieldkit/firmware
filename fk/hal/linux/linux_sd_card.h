#pragma once

#include "hal/sd_card.h"

namespace fk {

class LinuxSdCard : public SdCard {
public:
    LinuxSdCard();

public:
    bool begin() override;
    bool append_logs(circular_buffer<char> &buffer) override;
    SdCardFile *open(const char *name, Pool &pool) override;
    bool format() override;

};

class LinuxSdCardFile : public SdCardFile {
public:
    int32_t write(uint8_t const *buffer, size_t size) override;
    size_t file_size() override;
    bool close() override;
    operator bool() const override {
        return false;
    }

};

}
