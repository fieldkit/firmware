#include "factory_wipe.h"
#include "storage/storage.h"
#include "config.h"

namespace fk {

FK_DECLARE_LOGGER("factory");

FactoryWipe::FactoryWipe(Buttons *buttons, Storage *storage) : buttons_(buttons), storage_(storage) {
}

bool FactoryWipe::wipe_if_necessary() {
    if (buttons_->number_pressed() == 0) {
        return true;
    }

    loginfo("possible factory wipe...");

    auto wipe = false;
    auto started = fk_uptime();
    while (buttons_->number_pressed() > 0) {
        fk_delay(100);
        if (!wipe && fk_uptime() - started > FactoryWipeButtonDuration) {
            loginfo("will wipe on release!");
            wipe = true;
        }
    }

    if (!wipe) {
        return true;
    }

    loginfo("factory wipe!");

    return storage_->clear();
}

}
