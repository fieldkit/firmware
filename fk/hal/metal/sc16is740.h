#pragma once

#include "common.h"
#include "hal/board.h"
#include "io.h"

#if defined(ARDUINO)

namespace fk {

class Sc16is740 : public Reader {
private:
    AcquireTwoWireBus *acquire_bus_;

public:
    explicit Sc16is740(AcquireTwoWireBus *acquire_bus);

public:
    bool begin(uint32_t baud);
    int32_t available_for_read();
    int32_t available_for_write();
    bool read_fifo(uint8_t *buffer, size_t size);
    bool write_fifo(uint8_t const *buffer, size_t size);
    bool write(const char *line);

private:
    bool write_register(uint8_t reg, uint8_t value);
    bool read_register(uint8_t reg, uint8_t &value);

public:
    int32_t read(uint8_t *buffer, size_t size) override;

};

}

#endif
