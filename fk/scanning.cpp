#include "scanning.h"
#include "eeprom.h"
#include "state.h"
#include "config.h"

namespace fk {

FK_DECLARE_LOGGER("scan");

ModuleScanning::ModuleScanning(ModMux *mm) : mm_(mm) {
}

bool ModuleScanning::available() {
    // This checks for the presence of the multiplexing chips on the backplane.
    // If those aren't there then we just bail psuedo-succesfully with zero modules.
    return mm_->available();
}

static bool add_virtual_module(FoundModuleCollection &headers, uint16_t kind) {
    ModuleHeader header;
    header.manufacturer = FK_MODULES_MANUFACTURER;
    header.kind = kind;
    header.version = 0x1;
    header.crc = fk_module_header_sign(&header);

    headers.emplace_back(FoundModule{
        .position = 0xff,
        .header = header,
    });

    loginfo("[-] mk=%02" PRIx32 "%02" PRIx32 " v%" PRIu32 "", header.manufacturer, header.kind, header.version);

    return true;
}

nonstd::optional<FoundModuleCollection> ModuleScanning::scan(Pool &pool) {
    FoundModuleCollection found(pool);

    if (!available()) {
        return nonstd::optional<FoundModuleCollection>(found);
    }

    loginfo("scanning modules...");

    // Take ownership over the module bus.
    auto module_bus = get_board()->i2c_module();
    mm_->enable_all_modules();

    for (uint8_t i = 0; i < MaximumNumberOfModules; ++i) {
        if (!mm_->choose(i)) {
            return nonstd::nullopt;
        }

        ModuleHeader header;
        ModuleEeprom eeprom{ module_bus };
        bzero(&header, sizeof(ModuleHeader));
        if (!eeprom.read_header(header)) {
            continue;
        }

        if (fk_module_header_valid(&header)) {
            loginfo("[%d] mk=%02" PRIx32 "%02" PRIx32 " v%" PRIu32, i, header.manufacturer, header.kind, header.version);

            found.emplace_back(FoundModule{
                .position = (uint8_t)i,
                .header = header,
            });
        }
        else {
            logwarn("[%d] mk=%02" PRIx32 "%02" PRIx32 " v%" PRIu32, i, header.manufacturer, header.kind, header.version);
        }
    }

    // If any virtual modules are enabled, sneak the modules into the final
    // position. Right now we don't have a backplane that will support this many
    // modules anyway, still make sure we're safe.
    if (fk_config().readings.enable_diagnostics_module) {
        FK_ASSERT(add_virtual_module(found, FK_MODULES_KIND_DIAGNOSTICS));
    }
    if (fk_config().readings.enable_random_module) {
        FK_ASSERT(add_virtual_module(found, FK_MODULES_KIND_RANDOM));
    }

    loginfo("done (%zd modules)", found.size());

    return nonstd::optional<FoundModuleCollection>(found);
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
