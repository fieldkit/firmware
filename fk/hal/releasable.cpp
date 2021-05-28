#include <exchange.h>

#include "hal/mutex.h"

namespace fk {

Lock::Lock() : releasable_(nullptr) {
}

Lock::Lock(Releasable *releasable) : releasable_(releasable) {
}

Lock::Lock(Lock &&rhs) : releasable_(exchange(rhs.releasable_, nullptr)) {
}

Lock::~Lock() {
    if (releasable_ != nullptr) {
        releasable_->release();
        releasable_ = nullptr;
    }
}

} // namespace fk
