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

bool ModuleConfigurer::configure(ModulePosition position, ModuleHeader header) {
    fk_uuid_generate(&header.id);

    if (!scanning_->configure(position, header)) {
        logerror("[%d] unable to configure module", position.integer());
        return false;
    }

    loginfo("[%d] configured mk=%02" PRIx32 "%02" PRIx32 "", position.integer(), header.manufacturer, header.kind);

    return true;
}

}
