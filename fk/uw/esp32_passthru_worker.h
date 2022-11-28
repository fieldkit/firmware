#pragma once
#if defined(__SAMD51__) && defined(FK_NETWORK_ESP32)

#include "worker.h"

namespace fk {

class Esp32PassthruWorker : public Worker {
public:
    Esp32PassthruWorker();

public:
    void run(Pool &pool) override;

public:
    const char *name() const override {
        return "esp32";
    }
};

FK_ENABLE_TYPE_NAME(Esp32PassthruWorker);

} // namespace fk

#endif