#pragma once

#if defined(linux)

#include "hal/hal.h"

namespace fk {

class LinuxIPC : public IPC {
public:
    LinuxIPC();

public:
    bool available() override;
    bool begin() override;
    bool enqueue(void *ptr, uint32_t to) override;
    bool dequeue(void **ptr, uint32_t to) override;
    bool enqueue_data(StateChange *ptr, uint32_t to) override;
    bool dequeue_data(StateChange **ptr, uint32_t to) override;

public:
    bool launch_worker(Worker *worker) override;

};

}

#endif
