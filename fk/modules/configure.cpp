#include "modules/configure.h"

namespace fk {

FK_DECLARE_LOGGER("configure");

ModuleConfigurer::ModuleConfigurer(ModuleScanning &scanning) : scanning_(&scanning) {
}

bool ModuleConfigurer::weather(uint8_t position) {
    ModuleHeader header = {
        .manufacturer = FK_MODULES_MANUFACTURER,
        .kind = FK_MODULES_KIND_WEATHER,
        .version = 0x01,
        .id = { 0 },
    };

    if (!configure(position, header)) {
        return false;
    }

    loginfo("[%d] configured weather", position);

    return true;
}

bool ModuleConfigurer::water(uint8_t position) {
    ModuleHeader header = {
        .manufacturer = FK_MODULES_MANUFACTURER,
        .kind = FK_MODULES_KIND_WATER,
        .version = 0x01,
        .id = { 0 },
    };

    if (!configure(position, header)) {
        return false;
    }

    loginfo("[%d] configured water", position);

    return true;
}

bool ModuleConfigurer::distance(uint8_t position) {
    ModuleHeader header = {
        .manufacturer = FK_MODULES_MANUFACTURER,
        .kind = FK_MODULES_KIND_DISTANCE,
        .version = 0x01,
        .id = { 0 },
    };

    if (!configure(position, header)) {
        return false;
    }

    loginfo("[%d] configured distance", position);

    return true;
}

bool ModuleConfigurer::erase(uint8_t position) {
    if (!scanning_->erase(position)) {
        logerror("[%d] unable to configure module", position);
        return false;
    }

    loginfo("[%d] erased", position);

    return true;
}

bool ModuleConfigurer::configure(uint8_t position, ModuleHeader &header) {
    fk_uuid_generate(&header.id);

    if (!scanning_->configure(position, header)) {
        logerror("[%d] unable to configure module", position);
        return false;
    }

    return true;
}

}
