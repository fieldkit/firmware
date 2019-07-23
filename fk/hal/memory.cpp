#include <algorithm>

#include "hal/memory.h"
#include "hal/metal/metal_memory.h"
#include "hal/linux/linux_memory.h"
#include "board.h"
#include "config.h"

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

bool StatisticsMemory::begin() {
    return target_->begin();
}

flash_geometry_t StatisticsMemory::geometry() const {
    return target_->geometry();
}

size_t StatisticsMemory::read(uint32_t address, uint8_t *data, size_t length) {
    statistics_.nreads++;
    statistics_.bytes_read += length;
    return target_->read(address, data, length);
}

size_t StatisticsMemory::write(uint32_t address, const uint8_t *data, size_t length) {
    statistics_.nwrites++;
    statistics_.bytes_wrote += length;
    return target_->write(address, data, length);
}

size_t StatisticsMemory::erase_block(uint32_t address) {
    FK_ASSERT(geometry().is_address_valid(address));

    statistics_.nerases++;
    return target_->erase_block(address);
}

memory_statistics_t &StatisticsMemory::statistics() {
    return statistics_;
}

void StatisticsMemory::log_statistics() const {
    loginfo("%d reads (%d bytes) %d writes (%d bytes) %d erases",
            statistics_.nreads, statistics_.bytes_read,
            statistics_.nwrites, statistics_.bytes_wrote,
            statistics_.nerases);
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

#endif

BankedDataMemory memory{ bank_pointers, MemoryFactory::NumberOfDataMemoryBanks };

DataMemory **MemoryFactory::get_data_memory_banks() {
    return bank_pointers;
}

DataMemory *MemoryFactory::get_data_memory() {
    return &memory;
}

}
