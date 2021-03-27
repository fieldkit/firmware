#include "assert.h"
#include "logging.h"
#include "platform.h"

#ifdef __cplusplus
extern "C" {
#endif

void __cfy_assert(const char *assertion, const char *file, int line) {
    logerrorf("assertion", "\"%s\" failed: file \"%s\", line %d", assertion, file, line);
    alogging_platform_abort();
    /* NOTREACHED */
}

#ifdef __cplusplus
}
#endif
