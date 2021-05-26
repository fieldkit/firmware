#include "memory.h"
#include "config.h"
#include "platform.h"

namespace fk {

FK_DECLARE_LOGGER("memory");

constexpr size_t SizeOfStandardPagePool = 17;

struct StandardPages {
    void *base{ nullptr };
    int32_t available{ 0 };
    const char *owner{ nullptr };
    uint32_t allocated{ 0 };
};

static StandardPages pages[SizeOfStandardPagePool];
static size_t pages_allocated = 0u;
static size_t highwater = 0u;

void fk_standard_page_initialize() {
    FK_DISABLE_IRQ();

    if (pages_allocated == 0) {
        auto number_pages = SizeOfStandardPagePool;
        auto memory = reinterpret_cast<uint8_t*>(fk_malloc(StandardPageSize * number_pages));

        FK_ASSERT(number_pages <= SizeOfStandardPagePool);
        FK_ASSERT(number_pages > 0);

        for (auto i = 0u; i < number_pages; ++i) {
            pages[i].base = memory;
            pages[i].available = true;
            pages[i].owner = nullptr;
            pages[i].allocated = 0;
            memory += StandardPageSize;
        }

        pages_allocated = number_pages;
    }

    FK_ENABLE_IRQ();

    loginfo("allocated %zd pages (%zd bytes)", pages_allocated, pages_allocated * StandardPageSize);
}

void fk_oom() {
    logerror("oom!");

    for (auto i = 0u; i < SizeOfStandardPagePool; ++i) {
        if (!pages[i].available && pages[i].base != nullptr) {
            logerror("[%2d] owner = %s allocated=%" PRIu32,
                        i, pages[i].owner, pages[i].allocated);
        }
    }
}

static inline bool atomic_compare_exchange(int32_t *ptr, int32_t compare, int32_t exchange) {
    return __atomic_compare_exchange_n(ptr, &compare, exchange, 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST);
}

static inline void atomic_store(int32_t *ptr, int32_t value) {
    __atomic_store_n(ptr, value, __ATOMIC_SEQ_CST);
}

void *fk_standard_page_malloc(size_t size, const char *name) {
    FK_ASSERT(size == StandardPageSize);

    if (pages_allocated == 0) {
        fk_standard_page_initialize();
    }

    auto selected = -1;

    for (auto i = 0u; i < SizeOfStandardPagePool; ++i) {
        if (atomic_compare_exchange(&pages[i].available, 1, 0)) {
            selected = i;
            if (i > highwater) {
                highwater = selected;
            }
            pages[selected].owner = name;
            #if defined(__SAMD51__)
            pages[selected].allocated = fk_uptime();
            #endif
            logverbose("[%2d] malloc '%s'", selected, name);
            return pages[selected].base;
        }
    }

    fk_oom();

    FK_ASSERT(false);
    return nullptr;
}

void fk_standard_page_free(void *ptr) {
    const char *owner = "<unknown>";
    auto selected = -1;

    for (auto i = 0u; i < SizeOfStandardPagePool; ++i) {
        if (pages[i].base == ptr && pages[i].base != nullptr) {
            selected = i;

            owner = pages[selected].owner;
            pages[selected].owner = nullptr;

            #if defined(FK_ENABLE_MEMORY_GARBLE)
            fk_memory_garble(ptr, StandardPageSize);
            #else
            bzero(ptr, StandardPageSize);
            #endif

            atomic_store(&pages[selected].available, 1);
            break;
        }
    }

    if (selected == -1) {
        logerror("unknown page pointer: %p", ptr);
        FK_ASSERT(selected != -1);
    }

    logverbose("[%2d] free '%s'", selected, owner);

    return;
}

StandardPageMemInfo fk_standard_page_meminfo() {
    StandardPageMemInfo info = { 0, 0, 0, highwater };

    for (auto i = 0u; i < SizeOfStandardPagePool; ++i) {
        if (pages[i].available) {
            info.free++;
            info.total++;
        }
        else if (pages[i].base != nullptr) {
            info.used++;
            info.total++;
        }
    }

    return info;
}

void fk_standard_page_log() {
    for (auto i = 0u; i < SizeOfStandardPagePool; ++i) {
        if (!pages[i].available && pages[i].base != nullptr) {
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
