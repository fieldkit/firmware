#pragma once

#include "hal/metal/metal_network.h"

namespace fk {

#if defined(__SAMD51__) && defined(FK_NETWORK_ESP32)

class Esp32Network : public MetalNetwork {
public:
    Esp32Network();

public:
    bool begin(NetworkSettings settings, Pool *pool) override;
    PoolPointer<NetworkConnection> *open_connection(const char *scheme, const char *hostname, uint16_t port) override;

public:
    bool start_ap(NetworkSettings settings) override;
};

#endif

} // namespace fk