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
int32_t with_bank(DataMemory **memories, size_t size, uint32_t address, F fn) {
    auto bank_address = address;
    for (auto i = 0u; i < size; ++i) {
        auto &bank = *memories[i];
        auto g = bank.geometry();

        if (bank_address < g.total_size) {
            return fn(bank, bank_address);
        }

        bank_address -= g.total_size;
    }
    return 0;
}

bool BankedDataMemory::begin() {
    auto nbanks = 0;

    geometry_ = { 0, 0, 0, 0, 0, 0, 0 };

    for (size_t i = 0; i < size_; ++i) {
        auto bank = memories_[i];
        if (bank->begin()) {
            auto g = bank->geometry();
            if (geometry_.page_size == 0) {
                geometry_.page_size = g.page_size;
                geometry_.block_size = g.block_size;
                geometry_.prog_size = g.prog_size;
                geometry_.real_page_size = g.real_page_size;
                FK_ASSERT(g.prog_size > 0);
                FK_ASSERT(g.real_page_size > 0);
            }
            FK_ASSERT(geometry_.page_size == g.page_size);
            FK_ASSERT(geometry_.block_size == g.block_size);
            FK_ASSERT(geometry_.prog_size == g.prog_size);
            FK_ASSERT(geometry_.real_page_size == g.real_page_size);
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

int32_t BankedDataMemory::copy_page(uint32_t source, uint32_t destiny, size_t page_size, uint8_t *buffer, size_t buffer_size) {
    return with_bank(memories_, size_, source, [&](DataMemory &source_bank, uint32_t source_bank_address) -> int32_t {
        return with_bank(memories_, size_, destiny, [&](DataMemory &destiny_bank, uint32_t destiny_bank_address) -> int32_t {
            if (&source_bank == &destiny_bank) {
                return source_bank.copy_page(source_bank_address, destiny_bank_address, page_size, buffer, buffer_size);
            }
            else {
                FK_ASSERT(buffer_size >= page_size);

                logdebug("[0x%08" PRIx32 "] slow-copy to [0x%08" PRIx32 "]", source, destiny);

                auto read_err = source_bank.read(source_bank_address, buffer, page_size);
                if (read_err < 0) {
                    return read_err;
                }

                auto write_err = destiny_bank.write(destiny_bank_address, buffer, page_size);
                if (write_err < 0) {
                    return write_err;
                }

                return 0;
            }
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

TranslatingMemory::TranslatingMemory(DataMemory *target, int32_t offset_blocks) : target_(target), offset_(offset_blocks) {
}

bool TranslatingMemory::begin() {
    if (!target_->begin()) {
        return false;
    }

    block_size_ = target_->geometry().block_size;

    return true;
}

FlashGeometry TranslatingMemory::geometry() const {
    auto g = target_->geometry();
    g.nblocks -= offset_;
    return g;
}

int32_t TranslatingMemory::read(uint32_t address, uint8_t *data, size_t length, MemoryReadFlags flags) {
    return target_->read(translate(address), data, length, flags);
}

int32_t TranslatingMemory::write(uint32_t address, uint8_t const *data, size_t length, MemoryWriteFlags flags) {
    return target_->write(translate(address), data, length, flags);
}

int32_t TranslatingMemory::erase(uint32_t address, size_t length) {
    return target_->erase(translate(address), length);
}

int32_t TranslatingMemory::flush() {
    return target_->flush();
}

int32_t TranslatingMemory::copy_page(uint32_t source, uint32_t destiny, size_t page_size, uint8_t *buffer, size_t buffer_size) {
    return target_->copy_page(translate(source), translate(destiny), page_size, buffer, buffer_size);
}

uint32_t TranslatingMemory::translate(uint32_t address) {
    return address + offset_;
}

int32_t TranslatingMemory::execute(uint32_t *got, uint32_t *entry) {
    FK_ASSERT(false);
    return -1;
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
DataMemory *bank_pointers_normal[]{ &banks[0], &banks[1] };

#endif

MetalQspiMemory qspi_memory;
TranslatingMemory qspi_memory_translated{ &qspi_memory, -FK_MEMORY_QSPI_BASE };

#else

#if FK_MAXIMUM_NUMBER_OF_MEMORY_BANKS == 4

LinuxDataMemory banks[MemoryFactory::NumberOfDataMemoryBanks];
DataMemory *bank_pointers_normal[]{ &banks[0], &banks[1], &banks[2], &banks[3] };

#else

LinuxDataMemory banks[MemoryFactory::NumberOfDataMemoryBanks];
DataMemory *bank_pointers_normal[]{ &banks[0], &banks[1] };

#endif

LinuxDataMemory qspi_memory;
TranslatingMemory qspi_memory_translated{ &qspi_memory, -FK_MEMORY_QSPI_BASE };

#endif

BankedDataMemory banked_flash_memory{ bank_pointers_normal, MemoryFactory::NumberOfDataMemoryBanks };

#if defined(FK_ENABLE_PAGE_CACHE)
BasicPageCache<MemoryPageStore, 2048, 4> flash_cache{ { &banked_flash_memory } };
CachingMemory flash_caching_memory{ &banked_flash_memory, &flash_cache };
#endif

DataMemory **MemoryFactory::get_data_memory_banks() {
    return bank_pointers_normal;
}

DataMemory *MemoryFactory::get_data_memory() {
    #if defined(FK_ENABLE_PAGE_CACHE)
    return &flash_caching_memory;
    #else
    return &banked_flash_memory;
    #endif
}

ExecutableMemory *MemoryFactory::get_qspi_memory() {
    return &qspi_memory_translated;
}

}
