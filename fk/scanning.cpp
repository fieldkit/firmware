#include "scanning.h"

#include "modules/modules.h"

#if defined(ARDUINO)
namespace fk {

FK_DECLARE_LOGGER("scan");

ModuleScanning::ModuleScanning(ModMux *mm) : mm_(mm) {
}

class ModuleEeprom {
private:
    constexpr static uint8_t EepromAddress = 0x50;
    constexpr static size_t EepromPageSize = 32;
    constexpr static uint16_t HeaderAddress = 0x00;

private:
    TwoWireWrapper *wire_;

public:
    ModuleEeprom(TwoWireWrapper &wire) : wire_(&wire) {
    }

public:
    bool read_header(ModuleHeader &header) {
        static_assert(sizeof(ModuleHeader) == EepromPageSize, "ModuleHeader should be same size as one EEPROM page");

        auto address = HeaderAddress;
        if (!I2C_CHECK(wire_->write(EepromAddress, &address, sizeof(address)))) {
            return false;
        }

        if (!I2C_CHECK(wire_->read(EepromAddress, &header, sizeof(ModuleHeader)))) {
            return false;
        }

        return true;
    }

    bool write_header(ModuleHeader &header) {
        static_assert(sizeof(ModuleHeader) == EepromPageSize, "ModuleHeader should be same size as one EEPROM page");

        auto address = HeaderAddress;
        uint8_t buffer[sizeof(HeaderAddress) + sizeof(ModuleHeader)];
        memcpy(buffer,                         &address, sizeof(HeaderAddress));
        memcpy(buffer + sizeof(HeaderAddress), &header, sizeof(ModuleHeader));

        if (!I2C_CHECK(wire_->write(EepromAddress, buffer, sizeof(buffer)))) {
            return false;
        }

        return true;
    }

};

bool ModuleScanning::scan() {
    if (!mm_->available()) {
        return true;
    }

    // Take ownership over the module bus.
    auto module_bus = get_board()->i2c_module();

    mm_->enable_all_modules();

    loginfo("scanning modules");

    size_t number_of_modules = 0;

    for (size_t i = 0; i < MaximumNumberOfModules; ++i) {
        if (!mm_->choose(i)) {
            return false;
        }

        ModuleEeprom eeprom{ module_bus };
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
