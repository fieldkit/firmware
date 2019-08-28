#pragma once

#include "worker.h"

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
public:
    SelfCheckWorker() : Worker() {
    }

    SelfCheckWorker(Pool *pool) : Worker(pool) {
    }

public:
    void run(WorkerContext &wc, Pool &pool) override;

};

}
