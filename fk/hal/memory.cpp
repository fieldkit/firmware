#include "hal/memory.h"
#include "hal/metal/metal_memory.h"
#include "hal/linux/linux_memory.h"
#include "board.h"

namespace fk {

BankedDataMemory::BankedDataMemory(DataMemory **memories, size_t size) : memories_(memories), size_(size) {
}

template<typename F>
bool with_bank(DataMemory **memories, size_t size, uint32_t address, F fn) {
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

bool BankedDataMemory::read(uint32_t address, uint8_t *data, uint32_t length) {
    return with_bank(memories_, size_, address, [&](DataMemory &bank, uint32_t bank_address) {
        return bank.read(bank_address, data, length);
    });
}

bool BankedDataMemory::write(uint32_t address, const uint8_t *data, uint32_t length) {
    return with_bank(memories_, size_, address, [&](DataMemory &bank, uint32_t bank_address) {
        return bank.write(bank_address, data, length);
    });
}

bool BankedDataMemory::erase_block(uint32_t address) {
    return with_bank(memories_, size_, address, [&](DataMemory &bank, uint32_t bank_address) {
        return bank.erase_block(bank_address);
    });
}

#if defined(FK_HARDWARE_FULL)
MetalDataMemory banks[MemoryFactory::NumberOfDataMemoryBanks]{
    { SPI_FLASH_CS_BANK_1 },
    { SPI_FLASH_CS_BANK_2 },
    { SPI_FLASH_CS_BANK_3 },
    { SPI_FLASH_CS_BANK_4 },
};
#else
LinuxDataMemory banks[MemoryFactory::NumberOfDataMemoryBanks];
#endif

DataMemory *bank_pointers[]{ &banks[0], &banks[1], &banks[2], &banks[3] };
BankedDataMemory memory{ bank_pointers, 4 };

DataMemory **MemoryFactory::get_data_memory_banks() {
    return bank_pointers;
}

DataMemory *MemoryFactory::get_data_memory() {
    return &memory;
}

}
