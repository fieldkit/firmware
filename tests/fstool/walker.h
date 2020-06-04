#pragma once

#include "hal/linux/linux.h"
#include "hal/memory.h"
#include "storage/storage.h"

namespace fk {

class StorageWalker {
private:
    DataMemory *data_memory_;
    SequentialWrapper<BufferedPageMemory> memory_;
    Pool *pool_;

public:
    StorageWalker(DataMemory *data_memory, Pool &pool);

public:
    void walk();

};

}
