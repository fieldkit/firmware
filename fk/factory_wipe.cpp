#include "factory_wipe.h"
#include "storage/storage.h"
#include "config.h"

namespace fk {

FK_DECLARE_LOGGER("factory");

FactoryWipe::FactoryWipe(Display *display, Buttons *buttons, Storage *storage) : display_(display), buttons_(buttons), storage_(storage) {
}

bool FactoryWipe::wipe_if_necessary() {
    if (buttons_->number_pressed() == 0) {
        return true;
    }

    SimpleScreen screen = { "Hold for Reset" };

    display_->simple(screen);

    loginfo("possible factory wipe...");

    auto wipe = false;
    auto started = fk_uptime();
    while (buttons_->number_pressed() > 0) {
        fk_delay(100);
        if (!wipe && fk_uptime() - started > FactoryWipeButtonDuration) {
            loginfo("will wipe on release!");
            screen = { "Release for Reset" };
            display_->simple(screen);
            wipe = true;
        }
    }

    display_->off();

    if (!wipe) {
        return true;
    }

    loginfo("factory wipe!");

    if (!storage_->clear()) {
        return false;
    }

    return true;
}

}
