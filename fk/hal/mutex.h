#pragma once

#include "exchange.h"
#include "common.h"

namespace fk {

class Mutex;
class Lock;

class Releasable {
public:
    virtual bool release() = 0;
};

class Mutex : public Releasable {
public:
    virtual bool create() = 0;
    virtual Lock acquire(uint32_t to) = 0;
    virtual bool is_owner() = 0;

};

class RwLock : public Releasable {
public:
    virtual bool create() = 0;
    virtual Lock acquire_read(uint32_t to) = 0;
    virtual Lock acquire_write(uint32_t to) = 0;

};

class Lock {
private:
    Releasable *releasable_;

public:
    explicit Lock() : releasable_(nullptr) {
    }

    explicit Lock(Releasable *releasable) : releasable_(releasable) {
    }

    Lock(Lock &&rhs) : releasable_(exchange(rhs.releasable_, nullptr)) {
    }

    virtual ~Lock() {
        if (releasable_ != nullptr) {
            releasable_->release();
            releasable_ = nullptr;
        }
    }

public:
    Lock &operator=(Lock &&rhs) {
        if (this != &rhs) {
            releasable_ = rhs.releasable_;
            rhs.releasable_ = nullptr;
        }
        return *this;
    }

public:
    operator bool() {
        return releasable_ != nullptr;
    }
};

} // namespace fk
