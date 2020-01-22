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
static size_t highwater = 0u;

void fk_standard_page_initialize() {
    if (!initialized) {
        auto memory = reinterpret_cast<uint8_t*>(fk_malloc(StandardPageSize * SizeOfStandardPagePool));

        for (auto i = 0u; i < SizeOfStandardPagePool; ++i) {
            pages[i].base = memory;
            pages[i].available = true;
            pages[i].owner = nullptr;

            memory += StandardPageSize;
        }
    }

    loginfo("allocated %zd bytes", StandardPageSize * SizeOfStandardPagePool);

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

            if (i > highwater) {
                highwater = i;
            }
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

StandardPageMemInfo fk_standard_page_meminfo() {
    StandardPageMemInfo info = { 0, 0, highwater };

    for (auto i = 0u; i < SizeOfStandardPagePool; ++i) {
        if (pages[i].available) {
            info.free += 1;
        }
        info.total += 1;
    }

    return info;
}

void fk_standard_page_log() {
    for (auto i = 0u; i < SizeOfStandardPagePool; ++i) {
        if (!pages[i].available) {
            logdebug("[%2d] owner = %s", i, pages[i].owner);
        }
    }
}

} // namespace fk
