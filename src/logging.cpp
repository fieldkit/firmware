#include <cstdio>

#include "phylum.h"
#include "task_stack.h"

namespace phylum {

void phyinfof(const char *f, ...) {
    va_list args;
    va_start(args, f);
    valogfs(LogLevels::INFO, "phylum", get_task_stack()->get(), f, args);
    va_end(args);
}

void phyerrorf(const char *f, ...) {
    va_list args;
    va_start(args, f);
    valogfs(LogLevels::ERROR, "phylum", get_task_stack()->get(), f, args);
    va_end(args);
}

void phydebugf(const char *f, ...) {
    va_list args;
    va_start(args, f);
    valogfs(LogLevels::DEBUG, "phylum", get_task_stack()->get(), f, args);
    va_end(args);
}

void phywarnf(const char *f, ...) {
    va_list args;
    va_start(args, f);
    valogfs(LogLevels::WARN, "phylum", get_task_stack()->get(), f, args);
    va_end(args);
}

void phydebug_dump_memory(const char *prefix, const uint8_t *p, size_t size, ...) {
    va_list args;
    va_start(args, size);

    vprintf(prefix, args);
    for (auto i = 0u; i < size; ++i) {
        printf("%02x ", p[i]);
        if ((i + 1) % 32 == 0) {
            if (i + 1 < size) {
                printf("\n");
                vprintf(prefix, args);
            }
        }
    }
    printf(" (%zu bytes)\n", size);

    va_end(args);
}

} // namespace phylum
