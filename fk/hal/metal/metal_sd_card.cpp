#include <phylum/backend.h>
#include <backends/arduino_sd/arduino_sd.h>

#include <SPI.h>

#include "common.h"
#include "hal/board.h"
#include "hal/metal/metal_sd_card.h"

namespace fk {

FK_DECLARE_LOGGER("sdcard");

MetalSdCard::MetalSdCard() {
}

bool MetalSdCard::begin() {
    SPI2.begin();

    phylum::Geometry g;
    phylum::ArduinoSdBackend storage;
    if (!storage.initialize(g, PIN_SD_CS)) {
        logwarn("initialize failed");
        return false;
    }

    if (!storage.open()) {
        logwarn("open failed");
        return false;
    }

    return true;
}

}
