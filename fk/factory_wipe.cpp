#include "factory_wipe.h"
#include "storage/storage.h"
#include "config.h"

namespace fk {

FK_DECLARE_LOGGER("factory");

FactoryWipe::FactoryWipe(Storage &storage) : storage_(&storage) {
}

FactoryWipe::FactoryWipe(Display *display, Buttons *buttons, Storage *storage) : display_(display), buttons_(buttons), storage_(storage) {
}

bool FactoryWipe::wipe(ProgressCallbacks *progress) {
    loginfo("factory wipe!");

    if (!storage_->clear()) {
        return false;
    }

    if (false) {
        auto g = storage_->geometry();
        auto data_memory = MemoryFactory::get_data_memory();

        if (!data_memory->begin()) {
            return false;
        }

        auto consecutive_failures = 10;

        for (auto address = g.beginning(); address < g.end(); address += g.block_size) {
            if (!data_memory->erase_block(address)) {
                // These are usually bad blocks.
                logwarn("error erasing #%" PRIu32, address / g.block_size);

                consecutive_failures--;
                if (consecutive_failures == 0) {
                    return false;
                }
            }
            else {
                logverbose("erased #%" PRIu32, address / g.block_size);
                consecutive_failures = 0;
            }

            progress->progress(Operation::Wipe, (address / (float)g.end()) * 100.0f);
        }
    }

    loginfo("done");

    return true;
}

}
