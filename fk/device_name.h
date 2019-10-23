#pragma once

#include "common.h"
#include "pool.h"

namespace fk {

const char *fk_device_name_generate(Pool &pool);

const char *fk_device_name_printf(char *str, size_t size);

}
