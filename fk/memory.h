#pragma once

#include "common.h"

namespace fk {

void fk_standard_page_initialize();

void *fk_standard_page_malloc(size_t size, const char *name);

void fk_standard_page_free(void *ptr);

} // namespace fk
