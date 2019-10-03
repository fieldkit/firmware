// #include <phylum/backend.h>
// #include <backends/arduino_sd/arduino_sd.h>

#include "common.h"
#include "hal/board.h"
#include "hal/metal/metal_sd_card.h"

#if defined(__SAMD51__)

#include <SPI.h>

namespace fk {

#define SPI_SPEED SD_SCK_MHZ(50)

FK_DECLARE_LOGGER("sdcard");

MetalSdCard::MetalSdCard() {
}

bool MetalSdCard::begin() {
    SD_SPI.begin();

    SdFat sd(&SD_SPI);

    if (!sd.begin(PIN_SD_CS, SPI_SPEED)) {
        if (sd.card()->errorCode()) {
            logwarn("sd card error: 0x%x", sd.card()->errorCode());
        }
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

    return true;
}

}

#endif
