#include "modules/configure.h"

namespace fk {

FK_DECLARE_LOGGER("configure");

ModuleConfigurer::ModuleConfigurer(ModuleScanning &scanning) : scanning_(&scanning) {
}

bool ModuleConfigurer::erase(ModulePosition position) {
    if (!scanning_->erase(position)) {
        logerror("[%d] unable to configure module", position.integer());
        return false;
    }

    loginfo("[%d] erased", position.integer());

    return true;
}

bool ModuleConfigurer::provision(ModulePosition position, ModuleHeader header) {
    fk_uuid_generate(&header.id);

    if (!scanning_->provision(position, header)) {
        logerror("[%d] unable to provision module", position.integer());
        return false;
    }

    loginfo("[%d] provision mk=%02" PRIx32 "%02" PRIx32 "", position.integer(), header.manufacturer, header.kind);

    return true;
}

bool ModuleConfigurer::configure(ModulePosition position, uint8_t const *buffer, size_t size) {
    if (!scanning_->configure(position, buffer, size)) {
        return false;
    }
    return true;
}

}
