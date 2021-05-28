#pragma once

#include "common.h"

namespace fk {

int32_t fk_core_dump_check();

int32_t fk_core_dump_trigger();

int32_t fk_core_dump(const char *base_path);

int32_t fk_core_dump_tasks();

} // namespace fk
