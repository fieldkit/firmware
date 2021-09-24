#include <cstdio>

#include "phylum.h"
#include "task_stack.h"

namespace phylum {

void phyinfof(const char *f, ...) {
    va_list args;
    va_start(args, f);
    valogf(LogLevels::INFO, "phylum", f, args);
    va_end(args);
}

void phyerrorf(const char *f, ...) {
    va_list args;
    va_start(args, f);
    valogf(LogLevels::ERROR, "phylum", f, args);
    va_end(args);
}

void phyverbosef(const char *f, ...) {
    va_list args;
    va_start(args, f);
    valogf(LogLevels::VERBOSE, "phylum", f, args);
    va_end(args);
}

void phydebugf(const char *f, ...) {
    va_list args;
    va_start(args, f);
    valogf(LogLevels::DEBUG, "phylum", f, args);
    va_end(args);
}

void phywarnf(const char *f, ...) {
    va_list args;
    va_start(args, f);
    valogf(LogLevels::WARN, "phylum", f, args);
    va_end(args);
}

void phygraphf(const char *f, ...) {
    va_list args;
    va_start(args, f);
    valogfs(LogLevels::DEBUG, "phylum", "graph# ", f, args);
    va_end(args);
}

void phydebug_dump_memory(const char *prefix, const uint8_t *p, size_t size, ...) {
    va_list args;
    char rendered_prefix[32];
    va_start(args, size);
    vsnprintf(rendered_prefix, sizeof(rendered_prefix), prefix, args);
    va_end(args);


    printf("%s", rendered_prefix);
    for (auto i = 0u; i < size; ++i) {
        printf("%02x ", p[i]);
        if ((i + 1) % 32 == 0) {
            if (i + 1 < size) {
                printf("\n");
                printf("%s", rendered_prefix);
            }
        }
    }
    printf(" (%zu bytes)\n", size);
}

} // namespace phylum
