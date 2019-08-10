#include "scanning.h"
#include "eeprom.h"
#include "state.h"
#include "config.h"

namespace fk {

FK_DECLARE_LOGGER("scan");

ModuleScan::ModuleScan() {
}

ModuleScan::~ModuleScan() {
}

int32_t ModuleScan::size() const {
    return size_;
}

ModuleHeader const &ModuleScan::get(int32_t i) const {
    return headers_[i];
}

ModuleScanning::ModuleScanning(ModMux *mm) : mm_(mm) {
}

bool ModuleScanning::available() {
    // This checks for the presence of the multiplexing chips on the backplane.
    // If those aren't there then we just bail psuedo-succesfully with zero modules.
    return mm_->available();
}

bool ModuleScanning::scan(ModuleScan &scan) {
    if (!available()) {
        return true;
    }

    loginfo("scanning modules...");

    // Take ownership over the module bus.
    auto module_bus = get_board()->i2c_module();
    mm_->enable_all_modules();

    auto nmodules = 0;
    for (size_t i = 0; i < MaximumNumberOfModules; ++i) {
        if (!mm_->choose(i)) {
            return false;
        }

        auto &header = scan.headers_[i];
        ModuleEeprom eeprom{ module_bus };
        bzero(&header, sizeof(ModuleHeader));
        if (!eeprom.read_header(header)) {
            continue;
        }

        if (fk_module_header_valid(&header)) {
            loginfo("[%zd] mk=%02" PRIx32 "%02" PRIx32 " v%" PRIu32, i, header.manufacturer, header.kind, header.version);
        }
        else {
            logwarn("[%zd] mk=%02" PRIx32 "%02" PRIx32 " v%" PRIu32, i, header.manufacturer, header.kind, header.version);
        }

        nmodules++;
    }

    // If the random module is enabled, sneak the module into the final
    // position. Right now we don't have a backplane that will support this many
    // modules anyway, still make sure we're safe.
    if (fk_config().readings.enable_random_module) {
        auto &header = scan.headers_[MaximumNumberOfModules - 1];
        if (!fk_module_header_valid(&header)) {
            header.manufacturer = FK_MODULES_MANUFACTURER;
            header.kind = FK_MODULES_KIND_RANDOM;
            header.version = 0x1;
            fk_module_header_sign(&header);

            loginfo("[%d] mk=%02" PRIx32 "%02" PRIx32 " v%" PRIu32 "", 7, header.manufacturer, header.kind, header.version);

            nmodules++;
        }
    }

    scan.size_ = nmodules;

    loginfo("done (%d modules)", nmodules);

    return true;
}

bool ModuleScanning::configure(uint8_t position, ModuleHeader &header) {
    if (!available()) {
        return false;
    }

    if (!mm_->choose(position)) {
        return false;
    }

    // Take ownership over the module bus.
    auto module_bus = get_board()->i2c_module();
    mm_->enable_all_modules();

    ModuleEeprom eeprom{ module_bus };

    header.crc = fk_module_header_sign(&header);
    if (!eeprom.write_header(header)) {
        return false;
    }

    return true;
}

}
