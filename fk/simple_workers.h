#pragma once

#include "worker.h"
#include "self_check.h"

namespace fk {

class FsckWorker : public Worker {
public:
    FsckWorker();
    FsckWorker(Pool *pool);

public:
    void run(Pool &pool) override;

    const char *name() override {
        return "fsck";
    }

};

class WifiToggleWorker : public Worker {
public:
    WifiToggleWorker();
    WifiToggleWorker(Pool *pool);

public:
    void run(Pool &pool) override;

    const char *name() override {
        return "wifi";
    }

};

class SelfCheckWorker : public Worker {
private:
    SelfCheckCallbacks *callbacks_;

public:
    SelfCheckWorker(SelfCheckCallbacks *callbacks);
    SelfCheckWorker(SelfCheckCallbacks *callbacks, Pool *pool);

public:
    void run(Pool &pool) override;

    const char *name() override {
        return "self-check";
    }

private:
    bool stop_network();
    void check();


};

}
