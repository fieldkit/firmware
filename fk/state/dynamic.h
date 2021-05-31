#pragma once

#include "state/modules.h"

namespace fk {

namespace state {

class DynamicState {
private:
    Pool *pool_{ nullptr };
    AttachedModules *attached_{ nullptr };

public:
    DynamicState();
    DynamicState(DynamicState &&rhs);
    virtual ~DynamicState();

public:
    Pool *pool() const {
        return pool_;
    }

    AttachedModules *attached() const {
        return attached_;
    }

public:
    DynamicState &operator=(DynamicState &&rhs);

};

} // namespace state

} // namespace fk
