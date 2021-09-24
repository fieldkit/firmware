#include <phylum_fs.h>

#include "hal/flash.h"

namespace fk {

class PhylumFlashMemory : public phylum::flash_memory {
private:
    DataMemory *target_{ nullptr };
    phylum::working_buffers *buffers_{ nullptr };

public:
    PhylumFlashMemory(DataMemory *target, phylum::working_buffers *buffers);

public:
    bool begin();

public:
    size_t block_size() override;
    size_t number_blocks() override;
    size_t page_size() override;
    int32_t erase(uint32_t address, uint32_t length) override;
    int32_t write(uint32_t address, uint8_t const *data, size_t size) override;
    int32_t read(uint32_t address, uint8_t *data, size_t size) override;
    int32_t copy_page(uint32_t source, uint32_t destiny, size_t size) override;
};

} // namespace fk
