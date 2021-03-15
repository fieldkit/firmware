#pragma once

#include <dhara/map.h>
#include <dhara/nand.h>

#include "hal/memory.h"
#include "common.h"
#include "pool.h"

namespace fk {

class DharaNand;

typedef struct fk_dhara_t {
    struct dhara_nand dhara;
    DharaNand *dn;
} fk_dhara_t;

class Dhara {
private:
    fk_dhara_t nand_;

public:
    Dhara(Pool &pool);
    virtual ~Dhara();

public:
    bool begin();
};

} // namespace fk
