#include <algorithm>

#include "hal/memory.h"
#include "hal/metal/metal_memory.h"
#include "hal/metal/metal_qspi.h"
#include "hal/linux/linux_memory.h"
#include "board.h"
#include "config.h"
#include "utilities.h"

#include "storage/basic_page_cache.h"
#include "storage/memory_page_store.h"

#undef min
#undef max

namespace fk {

FK_DECLARE_LOGGER("memory");

BankedDataMemory::BankedDataMemory(DataMemory **memories, size_t size) : memories_(memories), size_(size) {
}

template<typename F>
size_t with_bank(DataMemory **memories, size_t size, uint32_t address, F fn) {
    auto bank_address = address;
    for (auto i = 0u; i < size; ++i) {
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

FlashGeometry BankedDataMemory::geometry() const {
    return geometry_;
}

int32_t BankedDataMemory::read(uint32_t address, uint8_t *data, size_t length, MemoryReadFlags flags) {
    return with_bank(memories_, size_, address, [&](DataMemory &bank, uint32_t bank_address) {
        return bank.read(bank_address, data, length, flags);
    });
}

int32_t BankedDataMemory::write(uint32_t address, uint8_t const *data, size_t length, MemoryWriteFlags flags) {
    return with_bank(memories_, size_, address, [&](DataMemory &bank, uint32_t bank_address) {
        return bank.write(bank_address, data, length, flags);
    });
}

int32_t BankedDataMemory::erase(uint32_t address, size_t length) {
    return for_each_block_between(address, length, geometry_.block_size, [=](uint32_t block_address) {
        return with_bank(memories_, size_, block_address, [&](DataMemory &bank, uint32_t bank_address) {
            return bank.erase(bank_address, geometry_.block_size);
        });
    });
}

int32_t BankedDataMemory::flush() {
    auto failed = false;
    for (size_t i = 0; i < size_; ++i) {
        auto bank = memories_[i];
        if (!bank->flush()) {
            failed = true;
        }
    }
    return !failed;
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

#else

MetalDataMemory banks[MemoryFactory::NumberOfDataMemoryBanks] {
    { SPI_FLASH_CS_BANK_1 },
    { SPI_FLASH_CS_BANK_2 },
};
DataMemory *bank_pointers[]{ &banks[0], &banks[1] };

#endif

MetalQspiMemory qspi_memory;

#else

#if FK_MAXIMUM_NUMBER_OF_MEMORY_BANKS == 4

LinuxDataMemory banks[MemoryFactory::NumberOfDataMemoryBanks];
DataMemory *bank_pointers[]{ &banks[0], &banks[1], &banks[2], &banks[3] };

#else

LinuxDataMemory banks[MemoryFactory::NumberOfDataMemoryBanks];
DataMemory *bank_pointers[]{ &banks[0], &banks[1] };

#endif

LinuxDataMemory qspi_memory;

#endif

BankedDataMemory banked_flash_memory{ bank_pointers, MemoryFactory::NumberOfDataMemoryBanks };

#if defined(FK_ENABLE_PAGE_CACHE)
BasicPageCache<MemoryPageStore, 2048, 4> flash_cache{ { &banked_flash_memory } };
CachingMemory flash_caching_memory{ &banked_flash_memory, &flash_cache };
#endif

DataMemory **MemoryFactory::get_data_memory_banks() {
    return bank_pointers;
}

DataMemory *MemoryFactory::get_data_memory() {
    #if defined(FK_ENABLE_PAGE_CACHE)
    return &flash_caching_memory;
    #else
    return &banked_flash_memory;
    #endif
}

ExecutableMemory *MemoryFactory::get_qspi_memory() {
    return &qspi_memory;
}

}
