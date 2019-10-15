#pragma once

#include "hal/sd_card.h"

#if defined(__SAMD51__)

#include <SdFat.h>
#include <sdios.h>

namespace fk {

class MetalSdCard : public SdCard {
public:
    MetalSdCard();

public:
    bool begin() override;
    bool append_logs(circular_buffer<char> &buffer) override;
    SdCardFile *open(const char *name, Pool &pool) override;
    bool format() override;

};

class MetalSdCardFile : public SdCardFile {
private:
    SdFile file_;

public:
    MetalSdCardFile();
    MetalSdCardFile(SdFile file);

public:
    int32_t write(uint8_t const *buffer, size_t size) override;
    size_t file_size() override;
    bool close() override;
    operator bool() const override {
        return file_.isFile();
    }

};

}

#endif
