#include "scanning.h"
#include "eeprom.h"
#include "state.h"

#if defined(ARDUINO)
namespace fk {

FK_DECLARE_LOGGER("scan");

ModuleScanning::ModuleScanning(ModMux *mm) : mm_(mm) {
}

bool ModuleScanning::scan() {
    // This checks for the presence of the multiplexing chips on the backplane.
    // If those aren't there then we just bail psuedo-succesfully with zero modules.
    if (!mm_->available()) {
        return true;
    }

    loginfo("scanning modules");

    // Reset our knowledge of the modules attached and the number of them.
    clear();

    // Take ownership over the module bus.
    auto module_bus = get_board()->i2c_module();

    mm_->enable_all_modules();

    for (size_t i = 0; i < MaximumNumberOfModules; ++i) {
        auto &header = headers_[i];

        if (!mm_->choose(i)) {
            return false;
        }

        ModuleEeprom eeprom{ module_bus };
        bzero(&header, sizeof(ModuleHeader));
        if (!eeprom.read_header(header)) {
            continue;
        }

        if (!fk_module_header_valid(&header)) {
            logerror("[%d] invalid header", i);
            continue;
        }

        loginfo("[%d] mk=%02x%02x version=%d", i,
                header.manufacturer, header.kind, header.version);

        number_of_modules_++;
    }

    loginfo("done (%d modules)", number_of_modules_);

    return true;
}

ModuleHeader &ModuleScanning::header(int32_t index) {
    return headers_[index];
}

size_t ModuleScanning::number_of_modules() const {
    return number_of_modules_;
}

void ModuleScanning::clear() {
    for (size_t i = 0; i < MaximumNumberOfModules; ++i) {
        headers_[i] = ModuleHeader{ };
    }
    number_of_modules_ = 0;
}

}

#endif
