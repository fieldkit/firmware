#include "scanning.h"

#include "eeprom.h"

#if defined(ARDUINO)
namespace fk {

FK_DECLARE_LOGGER("scan");

ModuleScanning::ModuleScanning(ModMux *mm) : mm_(mm) {
}

bool ModuleScanning::scan() {
    if (!mm_->available()) {
        return true;
    }

    loginfo("scanning modules");

    // Take ownership over the module bus.
    auto module_bus = get_board()->i2c_module();

    mm_->enable_all_modules();

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
