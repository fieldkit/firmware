#include <phylum/backend.h>
#include <backends/arduino_sd/arduino_sd.h>

#include "common.h"
#include "hal/board.h"
#include "hal/metal/metal_sd_card.h"

#include <SPI.h>

namespace fk {

FK_DECLARE_LOGGER("sdcard");

MetalSdCard::MetalSdCard() {
}

bool MetalSdCard::begin() {
    SD_SPI.begin();

    phylum::Geometry g;
    phylum::ArduinoSdBackend storage;
    if (!storage.initialize(g, PIN_SD_CS)) {
        return false;
    }

    return true;
}

}
