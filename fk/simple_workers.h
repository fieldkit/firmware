#pragma once

#include "worker.h"
#include "self_check.h"

namespace fk {

class FsckWorker {
public:
    void run(Pool &pool);

    const char *name() {
        return "fsck";
    }

};

class WifiToggleWorker {
public:
    void run(Pool &pool);

    const char *name() {
        return "wifi";
    }

};

class SelfCheckWorker {
private:
    SelfCheckCallbacks *callbacks_;

public:
    SelfCheckWorker(SelfCheckCallbacks *callbacks);

public:
    void run(Pool &pool);

    const char *name() {
        return "self-check";
    }

private:
    bool stop_network();
    void check();


};

}
