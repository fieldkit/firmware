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

    loginfo("done");

    return true;
}

}
