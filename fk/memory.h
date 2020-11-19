#pragma once

#include "common.h"

namespace fk {

void fk_standard_page_initialize();

void *fk_standard_page_malloc(size_t size, const char *name);

void fk_standard_page_free(void *ptr);

struct StandardPageMemInfo {
    size_t free;
    size_t total;
    size_t used;
    size_t highwater;
};

StandardPageMemInfo fk_standard_page_meminfo();

void fk_standard_page_log();

void fk_memory_garble(void *ptr, size_t size);

} // namespace fk
