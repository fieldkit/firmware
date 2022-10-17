#include "config.h"
#include "platform.h"
#include "utilities.h"
#include "modules/eeprom.h"
#include "modules/scanning.h"
#include "modules/shared/uuid.h"
#include "state.h"

#include <blake2b.h>

namespace fk {

FK_DECLARE_LOGGER("scan");

class CollectingListener : public ScanningListener {
private:
    FoundModuleCollection &collection_;

public:
    CollectingListener(FoundModuleCollection &collection) : collection_(collection) {
    }

public:
    int32_t scanned_module(ModulePosition const position, ModuleHeader const &header, Pool *pool) override {
        collection_.emplace(position, header);
        return 0;
    }
};

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

static bool add_virtual_module(ScanningListener *listener, uint16_t kind, Pool *pool) {
    logged_task lt{ "module[virt]" };

    ModuleHeader header;
    bzero(&header, sizeof(ModuleHeader));
    header.manufacturer = FK_MODULES_MANUFACTURER;
    header.kind = kind;
    header.version = 0x1;

    generate_unique_virtual_module_id(header);

    fk_uuid_formatted_t pretty_id;
    fk_uuid_sprintf(&header.id, &pretty_id);

    loginfo("[-] mk=%02" PRIx32 "%02" PRIx32 " v%" PRIu32 " %s", header.manufacturer, header.kind, header.version, pretty_id.str);

    auto err = listener->scanned_module(ModulePosition::Virtual, header, pool);
    if (err < 0) {
        return false;
    }

    return true;
}

bool ModuleScanning::try_scan_single_module(ScanningListener *listener, ModulePosition position, Pool &pool) {
    ModuleHeader header;
    bzero(&header, sizeof(ModuleHeader));
    if (!mm_->read_eeprom(ModuleEeprom::HeaderAddress, (uint8_t *)&header, sizeof(ModuleHeader))) {
        logwarn("[%d] unable to read eeprom", position.integer());
        return false;
    }

    if (!fk_module_header_valid(&header)) {
        auto expected = fk_module_header_sign(&header);
        logerror("[%d] invalid header (%" PRIx32 " != %" PRIx32 ")", position.integer(), expected, header.crc);

        auto err = listener->scanned_module(position, header, &pool);
        if (err < 0) {
            return false;
        }

        return true;
    }

    fk_uuid_formatted_t pretty_id;
    fk_uuid_sprintf(&header.id, &pretty_id);

    loginfo("[%d] mk=%02" PRIx32 "%02" PRIx32 " v%" PRIu32 " %s", position.integer(), header.manufacturer, header.kind, header.version,
            pretty_id.str);

    auto err = listener->scanned_module(position, header, &pool);
    if (err < 0) {
        return false;
    }

    return true;
}

int32_t ModuleScanning::scan(ScanningListener *listener, Pool &pool) {
    // If any virtual modules are enabled, sneak the modules into the final
    // position. Right now we don't have a backplane that will support this many
    // modules anyway, still make sure we're safe.
    if (ModulesEnableDiagnostics) {
        FK_ASSERT(add_virtual_module(listener, FK_MODULES_KIND_DIAGNOSTICS, &pool));
    }
    if (ModulesEnableRandom) {
        FK_ASSERT(add_virtual_module(listener, FK_MODULES_KIND_RANDOM, &pool));
    }

    // If the backplane isn't available, try and find a single module
    // on the bus.
    if (!available()) {
        loginfo("backplane unavailable, single mode scan");

        if (!try_scan_single_module(listener, ModulePosition::Solo, pool)) {
            logerror("[-] single module scan failed");
        }

        return 0;
    }

    for (auto position : mm_->available_positions()) {
        logged_task lt{ pool.sprintf("module[%d]", position.integer()) };

        if (!mm_->choose(position)) {
            logerror("[%d] error choosing", position.integer());
            return -1;
        }

        if (!try_scan_single_module(listener, position, pool)) {
            logwarn("[%d] no module", position.integer());
        }
    }

    if (!mm_->choose_nothing()) {
        logwarn("[-] error deselecting");
    }

    return 0;
}

tl::expected<FoundModuleCollection, Error> ModuleScanning::scan(Pool &pool) {
    FoundModuleCollection found(pool);
    CollectingListener listener{ found };

    auto err = scan(&listener, pool);
    if (err < 0) {
        logerror("scan");
        return tl::unexpected<Error>(Error::General);
    }

    return std::move(found);
}

bool ModuleScanning::provision(ModulePosition position, ModuleHeader &header) {
    if (position.requires_mod_mux()) {
        if (!available()) {
            logerror("[%d] requires modmux, unavailable", position.integer());
            return false;
        }

        if (!mm_->choose(position)) {
            logerror("[%d] error choosing module", position.integer());
            return false;
        }
    }

    auto module_bus = get_board()->i2c_module();
    ModuleEeprom eeprom{ module_bus };

    fk_delay(50);

    ModuleHeader existing_header;
    bzero(&existing_header, sizeof(ModuleHeader));
    if (!eeprom.read_header(existing_header)) {
        logerror("[%d] error reading header", position.integer());
        return false;
    }

    auto rewrite = !fk_module_header_valid(&existing_header) || memcmp(&existing_header, &header, sizeof(ModuleHeader)) != 0;
    if (rewrite) {
        logtrace("[%d] overwriting invalid header", position.integer());

        if (!eeprom.write_header(header)) {
            logerror("[%d] error writing header", position.integer());
            return false;
        }
    } else {
        loginfo("[%d] keeping existing header", position.integer());
        fk_logs_dump_memory_u8("header: ", (uint8_t *)&existing_header, sizeof(existing_header));
    }

    return true;
}

bool ModuleScanning::configure(ModulePosition position, uint8_t const *buffer, size_t size) {
    return true;
}

bool ModuleScanning::erase(ModulePosition position) {
    if (!available()) {
        return false;
    }

    if (!mm_->choose(position)) {
        logerror("[%d] error choosing module", position.integer());
        return false;
    }

    auto module_bus = get_board()->i2c_module();
    ModuleEeprom eeprom{ module_bus };

    fk_delay(50);

    eeprom.erase_all();

    return true;
}

} // namespace fk
