#include "syscalls_app.h"

#include "config.h"
#include "memory.h"

using namespace fk;

extern "C" {

void *svc_memory_alloc(void) {
    return fk_standard_page_malloc(StandardPageSize, "os");
}

uint32_t svc_memory_free(void *ptr) {
    fk_standard_page_free(ptr);
    return 0;
}

SVC_0_1(svc_memory_alloc, void*, RET_uint32_t);

SVC_1_1(svc_memory_free, uint32_t, void*, RET_uint32_t);

void *fkos_memory_alloc() {
    if (osi_in_task()) {
        return (void *)__svc_memory_alloc();
    }
    else {
        return (void *)svc_memory_alloc();
    }
}

void fkos_memory_free(void *ptr) {
    if (osi_in_task()) {
        __svc_memory_free(ptr);
    }
    else {
        svc_memory_free(ptr);
    }
}

}
