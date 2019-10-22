#include "firmware_manager.h"

#include "hal/flash.h"
#include "config.h"

namespace fk {

FK_DECLARE_LOGGER("firmware");

FirmwareManager::FirmwareManager() {
}

bool FirmwareManager::backup_bootloader(Pool &pool) {
    loginfo("copying active bootloader...");

    for (auto address = OtherBankAddress; address < OtherBankAddress + BootloaderSize; ) {
        loginfo("[0x%06" PRIx32 "] erasing", address);
        get_flash()->erase(address, CodeMemoryBlockSize / CodeMemoryPageSize);
        address += CodeMemoryBlockSize;
    }

    auto buffer = (uint8_t *)pool.malloc(CodeMemoryPageSize);

    for (auto offset = 0u; offset < BootloaderSize; ) {
        get_flash()->read(offset, buffer, CodeMemoryPageSize);
        get_flash()->write(OtherBankAddress + offset, buffer, CodeMemoryPageSize);
        offset += CodeMemoryPageSize;
    }

    loginfo("done!");

    return true;
}

}
