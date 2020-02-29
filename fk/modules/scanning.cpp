#include "config.h"
#include "platform.h"
#include "utilities.h"
#include "modules/eeprom.h"
#include "modules/scanning.h"
#include "modules/shared/uuid.h"
#include "state.h"

#include <phylum/blake2b.h>

namespace fk {

FK_DECLARE_LOGGER("scan");

ModuleScanning::ModuleScanning(ModMux *mm) : mm_(mm) {
}

bool ModuleScanning::available() {
    // This checks for the presence of the multiplexing chips on the backplane.
    // If those aren't there then we just bail psuedo-succesfully with zero modules.
    return mm_->available();
}

static bool generate_unique_virtual_module_id(ModuleHeader &header) {
    BLAKE2b hash;
    hash.reset(16);
    fk_serial_number_t sn;
    hash.update(&sn, sizeof(sn));
    hash.update(&header, sizeof(ModuleHeader));
    hash.finalize(&header.id, 16);
    return true;
}

static bool add_virtual_module(FoundModuleCollection &headers, uint16_t kind) {
    ModuleHeader header;
    bzero(&header, sizeof(ModuleHeader));
    header.manufacturer = FK_MODULES_MANUFACTURER;
    header.kind = kind;
    header.version = 0x1;

    generate_unique_virtual_module_id(header);

    headers.emplace(FoundModule{
        .position = ModMux::VirtualPosition,
        .header = header,
    });

    fk_uuid_formatted_t pretty_id;
    fk_uuid_sprintf(&header.id, &pretty_id);

    loginfo("[-] mk=%02" PRIx32 "%02" PRIx32 " v%" PRIu32 " %s", header.manufacturer, header.kind, header.version, pretty_id.str);

    return true;
}

tl::expected<FoundModuleCollection, Error> ModuleScanning::scan(Pool &pool) {
    FoundModuleCollection found(pool);

    // If any virtual modules are enabled, sneak the modules into the final
    // position. Right now we don't have a backplane that will support this many
    // modules anyway, still make sure we're safe.
    if (ModulesEnableDiagnostics) {
        FK_ASSERT(add_virtual_module(found, FK_MODULES_KIND_DIAGNOSTICS));
    }
    if (ModulesEnableRandom) {
        FK_ASSERT(add_virtual_module(found, FK_MODULES_KIND_RANDOM));
    }

    if (!available()) {
        return std::move(found);
    }

    DebuggerOfLastResort::get()->message("scanning");

    // Take ownership over the module bus.
    auto module_bus = get_board()->i2c_module();
    ModuleEeprom eeprom{ module_bus };

    for (auto i = 0u; i < MaximumNumberOfPhysicalModules; ++i) {
        if (!mm_->choose(i)) {
            logerror("[%d] error choosing", i);
            return tl::unexpected<Error>(Error::Bus);
        }

        ModuleHeader header;
        bzero(&header, sizeof(ModuleHeader));
        if (!eeprom.read_header(header)) {
            continue;
        }

        if (!fk_module_header_valid(&header)) {
            auto expected = fk_module_header_sign(&header);
            logerror("[%d] invalid header (%" PRIx32 " != %" PRIx32 ")", i, expected, header.crc);
            fk_dump_memory("HDR ", (uint8_t *)&header, sizeof(header));
            found.emplace(FoundModule{
                .position = (uint8_t)i,
                .header = header,
            });
            continue;
        }

        fk_uuid_formatted_t pretty_id;
        fk_uuid_sprintf(&header.id, &pretty_id);

        loginfo("[%d] mk=%02" PRIx32 "%02" PRIx32 " v%" PRIu32 " %s", i, header.manufacturer, header.kind, header.version, pretty_id.str);

        found.emplace(FoundModule{
            .position = (uint8_t)i,
            .header = header,
        });
    }

    if (!mm_->choose_nothing()) {
        logerror("[-] error deselecting");
        return std::move(found);
    }

    loginfo("done (%zd modules)", found.size());

    return std::move(found);
}

bool ModuleScanning::configure(uint8_t position, ModuleHeader &header) {
    if (!available()) {
        return false;
    }

    if (!mm_->choose(position)) {
        logerror("[%d] error choosing module", position);
        return false;
    }

    auto module_bus = get_board()->i2c_module();
    ModuleEeprom eeprom{ module_bus };

    fk_delay(50);

    ModuleHeader existing_header;
    bzero(&existing_header, sizeof(ModuleHeader));
    if (!eeprom.read_header(existing_header)) {
        logerror("[%d] error reading header", position);
        return false;
    }

    if (!fk_module_header_valid(&existing_header)) {
        logtrace("[%d] overwriting invalid header", position);

        if (!eeprom.write_header(header)) {
            logerror("[%d] error writing header", position);
            return false;
        }
    }
    else {
        loginfo("[%d] keeping existing header", position);
    }

    return true;
}

bool ModuleScanning::erase(uint8_t position) {
    if (!available()) {
        return false;
    }

    if (!mm_->choose(position)) {
        logerror("[%d] error choosing module", position);
        return false;
    }

    auto module_bus = get_board()->i2c_module();
    ModuleEeprom eeprom{ module_bus };

    fk_delay(50);

    eeprom.erase();

    return true;
}

}
