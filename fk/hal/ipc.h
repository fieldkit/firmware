#pragma once

#include "common.h"
#include "worker.h"

namespace fk {

class IPC {
public:
    virtual bool available();
    virtual bool begin();
    virtual bool enqueue(void *ptr, uint32_t to);
    virtual bool dequeue(void **ptr, uint32_t to);
    virtual bool enqueue_data(void *ptr, uint32_t to);
    virtual bool dequeue_data(void **ptr, uint32_t to);

public:
    virtual bool launch_worker(Worker *worker);

};

IPC *get_ipc();

}
