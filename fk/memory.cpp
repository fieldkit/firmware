#include "memory.h"
#include "config.h"

namespace fk {

constexpr size_t SizeOfStandardPagePool = 6;

struct StandardPages {
    void *base{ nullptr };
    uint8_t available;
};

static StandardPages pages[SizeOfStandardPagePool];

void fk_standard_page_initialize() {
    for (auto i = 0u; i < SizeOfStandardPagePool; ++i) {
        pages[i].base = fk_malloc(StandardPageSize);
        pages[i].available = true;
    }
}

void *fk_standard_page_malloc() {
    void *allocated = nullptr;

    for (auto i = 0u; i < SizeOfStandardPagePool; ++i) {
        if (pages[i].available) {
            allocated = pages[i].base;
            pages[i].available = false;
            break;
        }
    }

    FK_ASSERT(allocated != nullptr);

    return allocated;
}

void fk_standard_page_free(void *ptr) {
    auto success = false;

    for (auto i = 0u; i < SizeOfStandardPagePool; ++i) {
        if (pages[i].base == ptr) {
            bzero(pages[i].base, StandardPageSize);
            pages[i].available = true;
            success = true;
            break;
        }
    }

    FK_ASSERT(success);
}

} // namespace fk
