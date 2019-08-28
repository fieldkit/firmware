#pragma once

#include "worker.h"
#include "self_check.h"

namespace fk {

class FsckWorker : public Worker {
public:
    FsckWorker();
    FsckWorker(Pool *pool);

public:
    void run(WorkerContext &wc, Pool &pool) override;

};

class WifiToggleWorker : public Worker {
public:
    WifiToggleWorker();
    WifiToggleWorker(Pool *pool);

public:
    void run(WorkerContext &wc, Pool &pool) override;

};

class SelfCheckWorker : public Worker {
private:
    SelfCheckCallbacks *callbacks_;

public:
    SelfCheckWorker(SelfCheckCallbacks *callbacks);
    SelfCheckWorker(SelfCheckCallbacks *callbacks, Pool *pool);

public:
    void run(WorkerContext &wc, Pool &pool) override;

private:
    bool stop_network();
    void check();


};

}
