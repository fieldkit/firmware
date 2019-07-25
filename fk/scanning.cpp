#include "scanning.h"

#include "modules/modules.h"

#if defined(ARDUINO)
#include <Wire.h>

namespace fk {

FK_DECLARE_LOGGER("scan");

ModuleScanning::ModuleScanning(ModMux *mm) : mm_(mm) {
}

class ModuleEeprom {
private:
    constexpr static uint8_t EepromAddress = 0x50;
    constexpr static size_t EepromPageSize = 32;

private:
    TwoWireWrapper *wire_;

public:
    bool read_header(ModuleHeader &header) {
        static_assert(sizeof(ModuleHeader) == EepromPageSize, "ModuleHeader should be same size as one EEPROM page");

        Wire2.beginTransmission(EepromAddress);
        Wire2.write((uint8_t)0x00);
        Wire2.write((uint8_t)0x00);
        if (Wire2.endTransmission() != 0) {
            return false;
        }

        Wire2.requestFrom(EepromAddress, sizeof(ModuleHeader));
        uint8_t *ptr = (uint8_t *)&header;
        for (size_t i = 0; i < sizeof(ModuleHeader); ++i) {
            *ptr++ = Wire2.read();
        }
        if (Wire2.endTransmission() != 0) {
            return false;
        }

        return true;
    }

    bool write_header(ModuleHeader &header) {
        static_assert(sizeof(ModuleHeader) == EepromPageSize, "ModuleHeader should be same size as one EEPROM page");

        Wire2.beginTransmission(EepromAddress);
        Wire2.write((uint8_t)0x00);
        Wire2.write((uint8_t)0x00);
        uint8_t *ptr = (uint8_t *)&header;
        for (size_t i = 0; i < sizeof(ModuleHeader); ++i) {
            Wire2.write(*ptr++);
        }
        if (Wire2.endTransmission() != 0) {
            return false;
        }

        return true;
    }

};

bool ModuleScanning::scan() {
    if (!mm_->available()) {
        return true;
    }

    mm_->enable_all_modules();

    loginfo("scanning modules");

    size_t number_of_modules = 0;

    for (size_t i = 0; i < MaximumNumberOfModules; ++i) {
        if (!mm_->choose(i)) {
            return false;
        }

        ModuleEeprom eeprom;
        ModuleHeader header;
        bzero(&header, sizeof(ModuleHeader));
        if (!eeprom.read_header(header)) {
            continue;
        }

        if (!fk_module_header_valid(&header)) {
            logerror("[%d] invalid header", i);
            continue;
        }

        loginfo("[%d] mk=%02x%02x version=%d", header.manufacturer, header.kind, header.version);

        number_of_modules++;
    }

    loginfo("done (%d modules)", number_of_modules);

    return true;
}

}

#endif
