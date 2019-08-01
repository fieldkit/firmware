#include "scanning.h"
#include "eeprom.h"
#include "state.h"

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

bool ModuleScanning::scan(ModuleScan &scan) {
    // This checks for the presence of the multiplexing chips on the backplane.
    // If those aren't there then we just bail psuedo-succesfully with zero modules.
    if (!mm_->available()) {
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

        if (!fk_module_header_valid(&header)) {
            logerror("[%d] invalid header", i);
            continue;
        }

        loginfo("[%d] mk=%02x%02x v%d", i, header.manufacturer, header.kind, header.version);

        nmodules++;
    }

    scan.size_ = nmodules;

    loginfo("done (%d modules)", nmodules);

    return true;
}

}
