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

    NoopProgressCallbacks noop_progress;
    wipe(&noop_progress);

    return true;
}

bool FactoryWipe::wipe(ProgressCallbacks *progress) {
    loginfo("factory wipe!");

    if (!storage_->clear()) {
        return false;
    }

    auto g = storage_->geometry();
    auto data_memory = MemoryFactory::get_data_memory();

    if (!data_memory->begin()) {
        return false;
    }

    auto consecutive_failures = 10;

    for (auto address = g.beginning(); address < g.end(); address += g.block_size) {
        if (!data_memory->erase_block(address)) {
            logerror("" PRADDRESS " error erasing #%" PRIu32, address, address / g.block_size);

            consecutive_failures--;

            if (consecutive_failures == 0) {
                return false;
            }
        }
        else {
            loginfo("" PRADDRESS " erased #%" PRIu32, address, address / g.block_size);

            consecutive_failures = 0;
        }

        progress->progress(Operation::Wipe, (address / (float)g.end()) * 100.0f);
    }

    loginfo("done");

    return true;
}

}
