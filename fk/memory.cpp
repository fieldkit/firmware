#include "memory.h"
#include "config.h"

namespace fk {

FK_DECLARE_LOGGER("memory");

constexpr size_t SizeOfStandardPagePool = 18;

struct StandardPages {
    void *base{ nullptr };
    uint8_t available;
    const char *owner;
};

static StandardPages pages[SizeOfStandardPagePool];
static bool initialized = false;

void fk_standard_page_initialize() {
    if (!initialized) {
        for (auto i = 0u; i < SizeOfStandardPagePool; ++i) {
            pages[i].base = fk_malloc(StandardPageSize);
            pages[i].available = true;
            pages[i].owner = nullptr;
        }
    }

    initialized = true;
}

void *fk_standard_page_malloc(size_t size, const char *name) {
    void *allocated = nullptr;

    if (!initialized) {
        fk_standard_page_initialize();
    }

    FK_ASSERT(size == StandardPageSize);

    for (auto i = 0u; i < SizeOfStandardPagePool; ++i) {
        if (pages[i].available) {
            allocated = pages[i].base;
            pages[i].available = false;
            pages[i].owner = name;
            logdebug("[%2d] malloc '%s'", i, name);
            break;
        }
    }

    if (allocated == nullptr) {
        logerror("oom!");

        for (auto i = 0u; i < SizeOfStandardPagePool; ++i) {
            if (!pages[i].available) {
                logerror("[%2d] owner = %s", i, pages[i].owner);
            }
        }
    }

    FK_ASSERT(allocated != nullptr);

    return allocated;
}

void fk_standard_page_free(void *ptr) {
    auto success = false;

    for (auto i = 0u; i < SizeOfStandardPagePool; ++i) {
        if (pages[i].base == ptr) {
            logdebug("[%2d] free '%s'", i, pages[i].owner);
            bzero(pages[i].base, StandardPageSize);
            pages[i].available = true;
            pages[i].owner = nullptr;
            success = true;
            break;
        }
    }

    FK_ASSERT(success);
}

} // namespace fk
