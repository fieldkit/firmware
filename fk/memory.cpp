#include "memory.h"
#include "config.h"
#include "platform.h"

namespace fk {

FK_DECLARE_LOGGER("memory");

constexpr size_t SizeOfStandardPagePool = 18;

struct StandardPages {
    void *base{ nullptr };
    uint8_t available;
    const char *owner;
    uint32_t allocated{ 0 };
};

static StandardPages pages[SizeOfStandardPagePool];
static bool initialized = false;
static size_t highwater = 0u;

void fk_standard_page_initialize() {
    FK_DISABLE_IRQ();

    if (!initialized) {
        auto memory = reinterpret_cast<uint8_t*>(fk_malloc(StandardPageSize * SizeOfStandardPagePool));

        for (auto i = 0u; i < SizeOfStandardPagePool; ++i) {
            pages[i].base = memory;
            pages[i].available = true;
            pages[i].owner = nullptr;
            pages[i].allocated = 0;

            memory += StandardPageSize;
        }
    }

    initialized = true;

    FK_ENABLE_IRQ();

    loginfo("allocated %zd bytes", StandardPageSize * SizeOfStandardPagePool);
}

void *fk_standard_page_malloc(size_t size, const char *name) {
    FK_ASSERT(size == StandardPageSize);

    if (!initialized) {
        fk_standard_page_initialize();
    }

    auto selected = -1;

    FK_DISABLE_IRQ();

    for (auto i = 0u; i < SizeOfStandardPagePool; ++i) {
        if (pages[i].available) {
            selected = i;
            if (selected > (int32_t)highwater) {
                highwater = selected;
            }
            break;
        }
    }

    FK_ENABLE_IRQ();

    if (selected >= 0) {
        pages[selected].available = false;
        pages[selected].owner = name;
        #if defined(__SAMD51__)
        pages[selected].allocated = fk_uptime();
        #endif
        logdebug("[%2d] malloc '%s'", selected, name);
        return pages[selected].base;
    }
    else {
        logerror("oom!");

        for (auto i = 0u; i < SizeOfStandardPagePool; ++i) {
            if (!pages[i].available) {
                logerror("[%2d] owner = %s allocated=%" PRIu32,
                         i, pages[i].owner, pages[i].allocated);
            }
        }
    }

    FK_ASSERT(false);
    return nullptr;
}

void fk_standard_page_free(void *ptr) {
    auto selected = -1;

    FK_DISABLE_IRQ();

    for (auto i = 0u; i < SizeOfStandardPagePool; ++i) {
        if (pages[i].base == ptr) {
            selected = i;
            break;
        }
    }

    FK_ENABLE_IRQ();

    if (selected < 0) {
        FK_ASSERT(false);
        return;
    }

    logdebug("[%2d] free '%s'", selected, pages[selected].owner);

    #if defined(FK_ENABLE_MEMORY_GARBLE)
    fk_memory_garble(pages[selected].base, StandardPageSize);
    #else
    bzero(pages[selected].base, StandardPageSize);
    #endif
    pages[selected].available = true;
    pages[selected].owner = nullptr;

    return;
}

StandardPageMemInfo fk_standard_page_meminfo() {
    StandardPageMemInfo info = { 0, 0, 0, highwater };

    for (auto i = 0u; i < SizeOfStandardPagePool; ++i) {
        if (pages[i].available) {
            info.free++;
        }
        else {
            info.used++;
        }
        info.total++;
    }

    return info;
}

void fk_standard_page_log() {
    for (auto i = 0u; i < SizeOfStandardPagePool; ++i) {
        if (!pages[i].available) {
            logdebug("[%2d] owner = %s allocated=%" PRIu32, i, pages[i].owner, pages[i].allocated);
        }
    }
}

void fk_memory_garble(void *ptr, size_t size) {
    FK_ASSERT((size % 4) == 0);

    auto p = (uint32_t *)ptr;
    for (auto i = 0u; i < size / 4; ++i) {
        *p = 0xbeefdead;
    }
}

} // namespace fk
