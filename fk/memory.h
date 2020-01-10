#pragma once

#include "common.h"

namespace fk {

void *fk_standard_page_malloc();

void fk_standard_page_free(void *ptr);

} // namespace fk
