#include "factory_wipe.h"
#include "storage.h"

namespace fk {

FK_DECLARE_LOGGER("factory");

FactoryWipe::FactoryWipe(Buttons *buttons, DataMemory *memory) : buttons_(buttons), memory_(memory) {
}

bool FactoryWipe::wipe_if_necessary() {
    if (buttons_->number_pressed() == 0) {
        return true;
    }

    loginfo("buttons pressed, possible factory wipe...");

    auto wipe = false;
    auto started = fk_uptime();
    while (buttons_->number_pressed() > 0) {
        fk_delay(100);
        if (!wipe && fk_uptime() - started > 5000) {
            loginfo("will wipe on release!");
            wipe = true;
        }
    }

    if (!wipe) {
        return true;
    }

    loginfo("factory wipe!");

    Storage storage{ memory_ };

    return storage.clear();
}

}
