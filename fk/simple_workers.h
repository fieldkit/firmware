#pragma once

#include "worker.h"
#include "self_check.h"

namespace fk {

class FsckWorker {
public:
    void run(Pool &pool);

    uint8_t priority() const {
        return OS_PRIORITY_NORMAL;
    }

    const char *name() {
        return "fsck";
    }

};

class FactoryWipeWorker {
public:
    void run(Pool &pool);

    uint8_t priority() const {
        return OS_PRIORITY_NORMAL + 2;
    }

    const char *name() {
        return "wipe";
    }

};

class WifiToggleWorker {
public:
    enum class DesiredState {
        Enabled,
        Disabled,
        Toggle,
    };

private:
    DesiredState desired_;

public:
    WifiToggleWorker(DesiredState desired = DesiredState::Toggle);

public:
    void run(Pool &pool);

    uint8_t priority() const {
        return OS_PRIORITY_NORMAL;
    }

    const char *name() {
        return "wifi";
    }

};

class SelfCheckWorker {
private:
    SelfCheckCallbacks *callbacks_;

public:
    SelfCheckWorker(SelfCheckCallbacks &callbacks);

public:
    void run(Pool &pool);

    uint8_t priority() const {
        return OS_PRIORITY_NORMAL;
    }

    const char *name() {
        return "self-check";
    }

private:
    bool stop_network();
    void check();


};

}
