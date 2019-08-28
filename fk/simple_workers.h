#pragma once

#include "worker.h"
#include "self_check.h"

namespace fk {

class FsckWorker : public Worker {
public:
    FsckWorker() : Worker() {
    }

    FsckWorker(Pool *pool) : Worker(pool) {
    }

public:
    void run(WorkerContext &wc, Pool &pool) override;

};

class WifiToggleWorker : public Worker {
public:
    WifiToggleWorker() : Worker() {
    }

    WifiToggleWorker(Pool *pool) : Worker(pool) {
    }

public:
    void run(WorkerContext &wc, Pool &pool) override;

};

class SelfCheckWorker : public Worker {
private:
    SelfCheckCallbacks *callbacks_;

public:
    SelfCheckWorker(SelfCheckCallbacks *callbacks) : Worker(), callbacks_(callbacks) {
    }

    SelfCheckWorker(SelfCheckCallbacks *callbacks, Pool *pool) : Worker(pool), callbacks_(callbacks) {
    }

public:
    void run(WorkerContext &wc, Pool &pool) override;

};

}
