#include <os.h>
#include <alogging/alogging.h>

#pragma once

extern "C" {

void *fkos_memory_alloc();

void fkos_memory_free(void *ptr);

#define sys_vargs va_list

size_t fkos_logv(LogLevels level, const char *facility, const char *f, sys_vargs args);

size_t fkos_logf(LogLevels level, const char *facility, const char *f, ...) __attribute__((unused)) __attribute__((format(printf, 3, 4)));

}
