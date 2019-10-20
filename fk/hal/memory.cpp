#include <algorithm>

#include "hal/memory.h"
#include "hal/metal/metal_memory.h"
#include "hal/metal/metal_qspi.h"
#include "hal/linux/linux_memory.h"
#include "board.h"
#include "config.h"
#include "utilities.h"

#undef min
#undef max

namespace fk {

FK_DECLARE_LOGGER("memory");

BankedDataMemory::BankedDataMemory(DataMemory **memories, size_t size) : memories_(memories), size_(size) {
}

template<typename F>
size_t with_bank(DataMemory **memories, size_t size, uint32_t address, F fn) {
    auto bank_address = address;
    for (size_t i = 0; i < size; ++i) {
        auto &bank = *memories[i];
        auto g = bank.geometry();

        if (bank_address < g.total_size) {
            return fn(bank, bank_address);
        }

        bank_address -= g.total_size;
    }
    return false;
}

bool BankedDataMemory::begin() {
    auto nbanks = 0;

    geometry_ = { 0, 0, 0, 0 };

    for (size_t i = 0; i < size_; ++i) {
        auto bank = memories_[i];
        if (bank->begin()) {
            auto g = bank->geometry();
            if (geometry_.page_size == 0) {
                geometry_.page_size = g.page_size;
                geometry_.block_size = g.block_size;
            }
            FK_ASSERT(geometry_.page_size == g.page_size);
            FK_ASSERT(geometry_.block_size == g.block_size);
            geometry_.total_size += g.total_size;
            geometry_.nblocks += g.nblocks;
            nbanks++;
        }
    }
    return nbanks > 0;
}

flash_geometry_t BankedDataMemory::geometry() const {
    return geometry_;
}

size_t BankedDataMemory::read(uint32_t address, uint8_t *data, size_t length) {
    return with_bank(memories_, size_, address, [&](DataMemory &bank, uint32_t bank_address) {
        return bank.read(bank_address, data, length);
    });
}

size_t BankedDataMemory::write(uint32_t address, const uint8_t *data, size_t length) {
    return with_bank(memories_, size_, address, [&](DataMemory &bank, uint32_t bank_address) {
        return bank.write(bank_address, data, length);
    });
}

size_t BankedDataMemory::erase_block(uint32_t address) {
    return with_bank(memories_, size_, address, [&](DataMemory &bank, uint32_t bank_address) {
        return bank.erase_block(bank_address);
    });
}

size_t BankedDataMemory::flush() {
    auto failed = false;
    for (size_t i = 0; i < size_; ++i) {
        auto bank = memories_[i];
        if (!bank->flush()) {
            failed = true;
        }
    }
    return !failed;
}

SequentialMemory::SequentialMemory(DataMemory *memory) : memory_(memory) {
}

size_t SequentialMemory::read(uint32_t address, uint8_t *data, size_t length) {
    size_t nbytes = 0;

    auto g = memory_->geometry();
    auto p = data;
    auto remaining = length;

    FK_ASSERT(g.is_address_valid(address));

    auto rib = g.remaining_in_block(address);

    FK_ASSERT(length <= rib);

    while (nbytes != length) {
        auto left = g.remaining_in_page(address);
        auto reading = std::min<size_t>(remaining, left);
        if (!memory_->read(address, p, reading)) {
            return nbytes;
        }

        address += reading;
        remaining -= reading;
        nbytes += reading;
        p += reading;
    }

    return nbytes;
}

size_t SequentialMemory::write(uint32_t address, uint8_t *data, size_t length) {
    size_t nbytes = 0;

    auto g = memory_->geometry();
    auto p = data;
    auto remaining = length;

    FK_ASSERT(g.is_address_valid(address));

    auto rib = g.remaining_in_block(address);

    FK_ASSERT(length <= rib);

    while (nbytes != length) {
        auto left = g.remaining_in_page(address);
        auto writing = std::min<size_t>(remaining, left);
        if (!memory_->write(address, p, writing)) {
            return nbytes;
        }

        address += writing;
        remaining -= writing;
        nbytes += writing;
        p += writing;
    }

    return nbytes;
}

size_t SequentialMemory::flush() {
    return memory_->flush();
}

void MemoryStatistics::add(MemoryStatistics s) {
    nreads += s.nreads;
    nwrites += s.nwrites;
    nerases += s.nerases;
    bytes_read += s.bytes_read;
    bytes_wrote += s.bytes_wrote;
}

void MemoryStatistics::log() const {
    loginfo("%" PRIu32 " reads (%" PRIu32 " bytes) %" PRIu32 " writes (%" PRIu32 " bytes) %" PRIu32 " erases",
            nreads, bytes_read, nwrites, bytes_wrote, nerases);
}

bool StatisticsMemory::begin() {
    return target_->begin();
}

flash_geometry_t StatisticsMemory::geometry() const {
    return target_->geometry();
}

size_t StatisticsMemory::read(uint32_t address, uint8_t *data, size_t length) {
    statistics_.add_read(length);
    return target_->read(address, data, length);
}

size_t StatisticsMemory::write(uint32_t address, const uint8_t *data, size_t length) {
    statistics_.add_write(length);
    return target_->write(address, data, length);
}

size_t StatisticsMemory::erase_block(uint32_t address) {
    FK_ASSERT(geometry().is_address_valid(address));

    statistics_.nerases++;
    return target_->erase_block(address);
}

size_t StatisticsMemory::flush() {
    return target_->flush();
}

MemoryStatistics &StatisticsMemory::statistics() {
    return statistics_;
}

MemoryPageStore::MemoryPageStore(DataMemory *target) : target_(target) {
}

bool MemoryPageStore::load_page(uint32_t address, uint8_t *ptr, size_t size) {
    auto page_size = target_->geometry().page_size;
    auto page_address = ((uint32_t)(address / page_size)) * page_size;
    FK_ASSERT(page_size == size);
    auto rv = target_->read(page_address, ptr, size) == size;
    // fk_dump_memory("RD-PAGE ", ptr, page_size);
    return rv;
}

bool MemoryPageStore::save_page(uint32_t address, uint8_t const *ptr, size_t size) {
    auto page_size = target_->geometry().page_size;
    auto page_address = ((uint32_t)(address / page_size)) * page_size;
    FK_ASSERT(page_size == size);
    // fk_dump_memory("WR-PAGE ", ptr, page_size);
    return target_->write(page_address, ptr, size) == size;
}

void CachedPage::mark_dirty(uint16_t offset, uint16_t length) {
    dirty_start = std::min<uint16_t>(offset, dirty_start);
    dirty_end = std::max<uint16_t>(offset + length, dirty_end);
    ts = fk_uptime() + 1;
}

bool CachedPage::dirty() const {
    return dirty_start != UINT16_MAX;
}

CachingMemory::CachingMemory(DataMemory *target, PageCache *cache) : target_(target), cache_(cache) {
}

bool CachingMemory::begin() {
    if (!target_->begin()) {
        return false;
    }

    if (!cache_->invalidate()) {
        return false;
    }

    return true;
}

flash_geometry_t CachingMemory::geometry() const {
    return target_->geometry();
}

size_t CachingMemory::read(uint32_t address, uint8_t *data, size_t length) {
    auto page_size = target_->geometry().page_size;
    auto page = cache_->get_page(address);
    if (page == nullptr) {
        logerror("page lookup failed (0x%" PRIx32 ")", address);
        return 0;
    }
    FK_ASSERT(page->ptr != nullptr);
    memcpy(data, page->ptr + (address % page_size), length);
    // fk_dump_memory("RD-DATA ", data, length);
    return length;
}

#if defined(linux)
static void verify_erased(uint32_t address, uint8_t *p, size_t length) {
    for (size_t i = 0; i < length; ++i) {
        if (*p != 0xff) {
            logerror("corruption: 0x%x", address);
            assert(*p == 0xff);
        }
        p++;
    }
}
#endif

size_t CachingMemory::write(uint32_t address, const uint8_t *data, size_t length) {
    auto page_size = target_->geometry().page_size;
    auto page = cache_->get_page(address);
    if (page == nullptr) {
        logerror("page lookup failed (0x%" PRIx32 ")", address);
        return 0;
    }
    FK_ASSERT(page->ptr != nullptr);

    auto offset = (address % page_size);
    auto p = page->ptr + offset;
    #if defined(linux)
    verify_erased(address, p, length);
    #endif

    memcpy(p, data, length);
    page->mark_dirty(offset, length);
    // fk_dump_memory("CM-PAGE ", page->ptr, page_size);
    return length;
}

size_t CachingMemory::erase_block(uint32_t address) {
    if (!target_->erase_block(address)) {
        return false;
    }

    FK_ASSERT(cache_->invalidate(address));

    return true;
}

size_t CachingMemory::flush() {
    if (!target_->flush()) {
        logerror("memory flush failed");
        return false;
    }

    if (!cache_->flush()) {
        logerror("cache flush failed");
        return false;
    }

    return true;
}

#if defined(FK_HARDWARE_FULL)

#if FK_MAXIMUM_NUMBER_OF_MEMORY_BANKS == 4

MetalDataMemory banks[MemoryFactory::NumberOfDataMemoryBanks] {
    { SPI_FLASH_CS_BANK_1 },
    { SPI_FLASH_CS_BANK_2 },
    { SPI_FLASH_CS_BANK_3 },
    { SPI_FLASH_CS_BANK_4 },
};
DataMemory *bank_pointers[]{ &banks[0], &banks[1], &banks[2], &banks[3] };

#elif FK_MAXIMUM_NUMBER_OF_MEMORY_BANKS == 2

MetalDataMemory banks[MemoryFactory::NumberOfDataMemoryBanks] {
    { SPI_FLASH_CS_BANK_1 },
    { SPI_FLASH_CS_BANK_2 },
};
DataMemory *bank_pointers[]{ &banks[0], &banks[1] };

#elif FK_MAXIMUM_NUMBER_OF_MEMORY_BANKS == 1

MetalDataMemory banks[MemoryFactory::NumberOfDataMemoryBanks] {
    { SPI_FLASH_CS_BANK_1 },
};
DataMemory *bank_pointers[]{ &banks[0] };

#endif

MetalQspiMemory qspi_memory;

#else

#if FK_MAXIMUM_NUMBER_OF_MEMORY_BANKS == 4

LinuxDataMemory banks[MemoryFactory::NumberOfDataMemoryBanks];
DataMemory *bank_pointers[]{ &banks[0], &banks[1], &banks[2], &banks[3] };

#elif FK_MAXIMUM_NUMBER_OF_MEMORY_BANKS == 2

LinuxDataMemory banks[MemoryFactory::NumberOfDataMemoryBanks];
DataMemory *bank_pointers[]{ &banks[0], &banks[1] };

#elif FK_MAXIMUM_NUMBER_OF_MEMORY_BANKS == 1

LinuxDataMemory banks[MemoryFactory::NumberOfDataMemoryBanks];
DataMemory *bank_pointers[]{ &banks[0] };

#endif

LinuxDataMemory qspi_memory;

#endif

template<typename PageStoreType, size_t PageSize, size_t N>
class BasicPageCache : public PageCache {
private:
    void *memory_;
    PageStoreType store_;
    CachedPage pages_[N];

public:
    BasicPageCache(PageStoreType store) : store_(store) {
        memory_ = fk_malloc(N * PageSize);
        for (size_t i = 0; i < N; ++i) {
            pages_[i] = { };
            pages_[i].ptr = ((uint8_t *)memory_) + i * PageSize;
        }
    }

public:
    CachedPage *get_page(uint32_t address) override {
        CachedPage *available = nullptr;
        CachedPage *old = nullptr;

        uint32_t page = address / PageSize;

        for (size_t i = 0; i < N; ++i) {
            auto p = &pages_[i];
            // logverbose("page[%zd] page=#%" PRIu32 " ts=%" PRIu32 " %s", i, p->page, p->ts, p->dirty() ? "dirty": "");
            if (p->ts == 0) {
                available = p;
                old = p;
            }
            else {
                if (p->page == page) {
                    // logverbose("existing page #%" PRIu32 " (%" PRIu32 ")", p->page, p->ts);
                    return p;
                }
                if (old == nullptr || old->ts > p->ts) {
                    old = p;
                }
            }
        }

        FK_ASSERT(old != nullptr);

        if (available == nullptr) {
            if (!flush(old)) {
                return nullptr;
            }
            available = old;
        }

        FK_ASSERT(available != nullptr);

        available->ts = fk_uptime() + 1;
        available->page = page;
        available->mark_clean();

        // logtrace("load page #%" PRIu32 " (%" PRIu32 ")", available->page, available->ts);

        if (!store_.load_page(page * PageSize, available->ptr, PageSize)) {
            return nullptr;
        }

        return available;
    }

    size_t invalidate(uint32_t address) override {
        uint32_t page = address / PageSize;
        for (size_t i = 0; i < N; ++i) {
            auto p = &pages_[i];
            if (p->page == page) {
                if (p->dirty()) {
                    logerror("invalidate DIRTY (%" PRIu32 ")", page);
                    FK_ASSERT(!p->dirty());
                }
                else if (p->ts > 0) {
                    logtrace("invalidate (%" PRIu32 ")", page);
                }
                p->mark_clean();
                p->ts = 0;
                p->page = 0;
            }
        }

        return true;
    }

    size_t invalidate() override {
        for (size_t i = 0; i < N; ++i) {
            auto p = &pages_[i];
            if (p->dirty()) {
                logerror("invalidate(all) DIRTY (%" PRIu32 ")", p->page);
                FK_ASSERT(!p->dirty());
            }
            else if (p->ts > 0) {
                logtrace("invalidate(all) (%" PRIu32 ")", p->page);
            }
            p->mark_clean();
            p->ts = 0;
            p->page = 0;
        }

        return true;
    }

    bool flush(CachedPage *page) override {
        if (!page->dirty()) {
            return true;
        }

        logdebug("flush #%" PRIu32 " dirty(%d - %d)", page->page, page->dirty_start, page->dirty_end);

        if (!store_.save_page(page->page * PageSize, page->ptr, PageSize)) {
            return false;
        }

        page->mark_clean();

        return true;
    }

    bool flush() override {
        for (size_t i = 0; i < N; ++i) {
            if (!flush(&pages_[i])) {
                logerror("flush page failed");
                return false;
            }
        }

        return true;
    }

};

BankedDataMemory banked_flash_memory{ bank_pointers, MemoryFactory::NumberOfDataMemoryBanks };
BasicPageCache<MemoryPageStore, 2048, 4> flash_cache{ { &banked_flash_memory } };
CachingMemory flash_caching_memory{ &banked_flash_memory, &flash_cache };

DataMemory **MemoryFactory::get_data_memory_banks() {
    return bank_pointers;
}

DataMemory *MemoryFactory::get_data_memory() {
    return &flash_caching_memory;
}

DataMemory *MemoryFactory::get_qspi_memory() {
    return &qspi_memory;
}

}
