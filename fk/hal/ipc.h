#pragma once

#include "common.h"
#include "worker.h"
#include "state.h"

namespace fk {

class IPC {
public:
    virtual bool available() = 0;
    virtual bool begin() = 0;
    virtual bool enqueue(void *ptr, uint32_t to) = 0;
    virtual bool dequeue(void **ptr, uint32_t to) = 0;

    virtual bool enqueue_data(StateChange *ptr, uint32_t to) = 0;
    virtual bool dequeue_data(StateChange **ptr, uint32_t to) = 0;

    template<typename T>
    bool enqueue_data(T fn) {
        return enqueue_data(new SimpleStateChange<T>(fn), 5000);
    }

public:
    virtual bool launch_worker(Worker *worker) = 0;

};

IPC *get_ipc();

}
