#pragma once

#include "common.h"

namespace fk {

class IPC {
public:
    bool begin();
    bool enqueue(void *ptr, uint32_t to);
    bool dequeue(void **ptr, uint32_t to);

};

IPC *get_ipc();

}
