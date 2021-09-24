#pragma once

#include "common.h"

namespace fk {

class Releasable {
public:
    virtual bool release() = 0;
};

class Lock {
private:
    Releasable *releasable_;

public:
    explicit Lock();
    explicit Lock(Releasable *releasable);
    Lock(Lock &&rhs);
    virtual ~Lock();

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

class Mutex;
class Lock;

class Mutex : public Releasable {
public:
    virtual bool create() = 0;
    virtual Lock acquire(uint32_t to) = 0;
    virtual bool is_owner() = 0;

public:
    template<typename TReturn, typename TFn>
    TReturn with(TFn work) {
        auto lock = this->acquire(UINT32_MAX);
        return work();
    }

};

class RwLock : public Releasable {
public:
    virtual bool create() = 0;
    virtual Lock acquire_read(uint32_t to) = 0;
    virtual Lock acquire_write(uint32_t to) = 0;

};

class NoopMutex : public Mutex {
public:
    bool create() override {
        return true;
    }

    Lock acquire(uint32_t to) override {
        return Lock{ this };
    }

    bool release() override {
        return true;
    }

    bool is_owner() override {
        return true;
    }
};

} // namespace fk
