#pragma once

#include "hal/metal/mdns.h"

#include <ArduinoMDNS.h>

namespace fk {

class MDNSPoolAllocator : public MDNSAllocator {
private:
    Pool *pool_;

public:
    explicit MDNSPoolAllocator(Pool *pool) : pool_(pool) {
    }

public:
    void *malloc(size_t size) override {
        return pool_->calloc(size);
    }

    void free(void *ptr) override {
    }
};

} // namespace fk
