#include "factory_wipe.h"
#include "storage/storage.h"
#include "config.h"

namespace fk {

FK_DECLARE_LOGGER("factory");

FactoryWipe::FactoryWipe(Storage &storage) : storage_(&storage) {
}

FactoryWipe::FactoryWipe(Display *display, Buttons *buttons, Storage *storage) : display_(display), buttons_(buttons), storage_(storage) {
}

bool FactoryWipe::wipe_if_necessary() {
    if (buttons_->number_pressed() == 0) {
        return true;
    }

    SimpleScreen screen = { "Hold for Reset" };

    display_->simple(screen);

    loginfo("possible factory wipe...");

    auto should_wipe = false;
    auto started = fk_uptime();
    while (buttons_->number_pressed() > 0) {
        fk_delay(100);
        if (!should_wipe && fk_uptime() - started > FactoryWipeButtonDuration) {
            loginfo("will wipe on release!");
            screen = { "Release for Reset" };
            display_->simple(screen);
            should_wipe = true;
        }
    }

    display_->off();

    if (!should_wipe) {
        return true;
    }

    wipe();

    return true;
}

bool FactoryWipe::wipe() {
    loginfo("factory wipe!");

    if (!storage_->clear()) {
        return false;
    }

    return true;
}

}
