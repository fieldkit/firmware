#pragma once

#include "fk.h"
#include "pool_pointer.h"

namespace fk {

typedef struct ip4_address {
    union {
        uint32_t dword;
        uint8_t bytes[4];
    } u;
} ip4_address;

enum class NetworkStatus {
    Error = 0,
    Ready,
    Off,
    Listening,
    Connected
};

enum class NetworkConnectionStatus {
    Connected,
    Disconnected
};

typedef struct NetworkSettings {
    bool valid;
    bool create;
    const char *ssid;
    const char *password;
    uint16_t port;
} NetworkSettings;

class NetworkConnection {
public:
    virtual ~NetworkConnection() {
    }

public:
    virtual NetworkConnectionStatus status() = 0;

    virtual bool available() = 0;

    virtual int32_t read(uint8_t *buffer, size_t size) = 0;

    virtual int32_t write(const uint8_t *buffer, size_t size) = 0;

    virtual int32_t writef(const char *str, ...) __attribute__((format(printf, 2, 3))) = 0;

    virtual int32_t vwritef(const char *str, va_list args) = 0;

    virtual int32_t write(const char *str) = 0;

    virtual int32_t socket() = 0;

    virtual uint32_t remote_address() = 0;

    virtual bool stop() = 0;

};

class NetworkListener {
public:
    virtual PoolPointer<NetworkConnection> *accept() = 0;

    virtual bool stop() = 0;

};

class Network {
public:
    virtual bool begin(NetworkSettings settings) = 0;

    virtual bool serve() = 0;

    virtual NetworkStatus status() = 0;

    virtual uint32_t ip_address() = 0;

    virtual void service() = 0;

    virtual PoolPointer<NetworkConnection> *open_connection(const char *hostname, uint16_t port) = 0;

    virtual PoolPointer<NetworkListener> *listen(uint16_t port) = 0;

    virtual bool stop() = 0;

    virtual bool enabled() = 0;

};

Network *get_network();

template<typename ConcreteWrapped, class... Args, typename ConcreteWrapee = StandardPoolWrapper<NetworkConnection, ConcreteWrapped, Args...>>
inline PoolPointer<NetworkConnection> *create_network_connection_wrapper(Args &&... args) {
    auto pool = create_standard_pool_inside(TypeName<ConcreteWrapped>::get());
    return new (pool) ConcreteWrapee(pool, std::forward<Args>(args)...);
}

template<typename ConcreteWrapped, class... Args, typename ConcreteWrapee = StandardPoolWrapper<NetworkListener, ConcreteWrapped, Args...>>
inline PoolPointer<NetworkListener> *create_network_listener_wrapper(Args &&... args) {
    auto pool = create_standard_pool_inside(TypeName<ConcreteWrapped>::get());
    return new (pool) ConcreteWrapee(pool, std::forward<Args>(args)...);
}

template<typename ConcreteWrapped, class... Args, typename ConcreteWrapee = WeakPoolWrapper<NetworkListener, ConcreteWrapped, Args...>>
inline PoolPointer<NetworkListener> *create_weak_network_listener_wrapper(Pool &pool, Args &&... args) {
    return new (pool) ConcreteWrapee(pool, std::forward<Args>(args)...);
}

FK_ENABLE_TYPE_NAME(NetworkConnection);
FK_ENABLE_TYPE_NAME(NetworkListener);

}
