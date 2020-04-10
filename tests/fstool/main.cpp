#include <inttypes.h>
#include <unistd.h>

#include "fk.h"
#include "hal/linux/linux.h"
#include "protobuf.h"

FK_DECLARE_LOGGER("main");

using namespace fk;

__int32_t main(__int32_t argc, const char **argv) {
    log_configure_level(LogLevels::INFO);
    fk_standard_page_initialize();

    loginfo("hello");

    loginfo("done");

    return 0;
}

extern "C" {

void osi_assert(const char *assertion, const char *file, int line) {
    fprintf(stderr, "\n\nassertion \"%s\" failed: file \"%s\", line %d\n", assertion, file, line);
    exit(2);
}

void fk_assert(const char *assertion, const char *file, int32_t line, const char *f, ...) {
    fprintf(stderr, "\n\nassertion \"%s\" failed: file \"%s\", line %d\n", assertion, file, line);
    exit(2);
}

}
