#pragma once

#if defined(FK_HARDWARE_FULL)

#include "hal/hal.h"

namespace fk {

class MetalIPC : public IPC {
public:
    MetalIPC();

public:
    bool available() override;
    bool begin() override;
    bool enqueue(void *ptr, uint32_t to) override;
    bool dequeue(void **ptr, uint32_t to) override;
    bool enqueue_data(void *ptr, uint32_t to) override;
    bool dequeue_data(void **ptr, uint32_t to) override;

public:
    bool launch_worker(Worker *worker) override;

};

}

#endif
