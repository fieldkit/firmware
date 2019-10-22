#pragma once

#include "pool.h"

namespace fk {

class FirmwareManager {
public:
    FirmwareManager();

public:
    bool backup_bootloader(Pool &pool);

};

}
