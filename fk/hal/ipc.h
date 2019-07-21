#pragma once

#include "common.h"
#include "worker.h"

namespace fk {

class IPC {
public:
    virtual bool available() = 0;
    virtual bool begin() = 0;
    virtual bool enqueue(void *ptr, uint32_t to) = 0;
    virtual bool dequeue(void **ptr, uint32_t to) = 0;
    virtual bool enqueue_data(void *ptr, uint32_t to) = 0;
    virtual bool dequeue_data(void **ptr, uint32_t to) = 0;

public:
    virtual bool launch_worker(Worker *worker) = 0;

};

IPC *get_ipc();

}
