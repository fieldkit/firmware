#include <os.h>
#include <alogging/alogging.h>

#pragma once

extern "C" {

void *fkos_memory_alloc();

void fkos_memory_free(void *ptr);

}
