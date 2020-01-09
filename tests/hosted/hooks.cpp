#include "tests.h"
#include "utilities.h"

extern "C" {

void osi_assert(const char *assertion, const char *file, int line) {
    FAIL() << "Assertion \"" << assertion << "\" failed. File: " << file << " Line: " << line;
}

void fk_assert(const char *assertion, const char *file, int32_t line, const char *f, ...) {
    FAIL() << "Assertion \"" << assertion << "\" failed. File: " << file << " Line: " << line;
}

}
