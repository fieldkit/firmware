#pragma once

#include <tiny_printf.h>

#include <gmock/gmock.h>

#include "hal/network.h"
#include "modules/scanning.h"

namespace fk {

class MockModuleScanning : public ModuleScanning {
public:
    MockModuleScanning() : ModuleScanning(nullptr) {
    }

public:
    MOCK_METHOD1(scan, tl::expected<FoundModuleCollection, Error>(Pool &pool));

};

class MockNetwork : public Network {
public:
    MOCK_METHOD1(begin, bool(NetworkSettings settings));
    MOCK_METHOD0(serve, bool());
    MOCK_METHOD0(status, NetworkStatus());
    MOCK_METHOD0(ip_address, uint32_t());
    MOCK_METHOD1(listen, PoolPointer<NetworkListener>*(uint16_t port));
    MOCK_METHOD0(service, void());
    MOCK_METHOD3(open_connection, PoolPointer<NetworkConnection>*(const char *scheme, const char *hostname, uint16_t port));
    MOCK_METHOD0(stop, bool());
    MOCK_METHOD0(enabled, bool());

};

class MockListener : public NetworkListener {
public:
    MOCK_METHOD0(accept, PoolPointer<NetworkConnection>*());
    MOCK_METHOD0(stop, bool());

};

class MockNetworkConnection : public NetworkConnection {
public:
    MOCK_METHOD0(status, NetworkConnectionStatus());
    MOCK_METHOD0(available, bool());
    MOCK_METHOD2(read, int32_t(uint8_t *buffer, size_t size));
    MOCK_METHOD2(write, int32_t(const uint8_t *buffer, size_t size));
    MOCK_METHOD1(write, int32_t(const char *str));
    MOCK_METHOD0(socket, int32_t());
    MOCK_METHOD0(stop, bool());

    int32_t writef(const char *str, ...) override {
        va_list args;
        va_start(args, str);
        auto needed = tiny_vsnprintf(nullptr, 0, str, args);
        va_end(args);
        auto buffer = (char *)malloc(needed + 1);
        va_start(args, str);
        tiny_vsnprintf(buffer, needed + 1, str, args);
        write(buffer);
        free(buffer);
        va_end(args);
        return 0;
    }

};

class FakeNetwork : public Network {
private:
    NetworkSettings settings_;
    bool enabled_{ false };

public:
    bool begin(NetworkSettings settings) override {
        settings_ = settings;
        return true;
    }

    bool serve() override {
        return true;
    }

    NetworkStatus status() override {
        return NetworkStatus::Connected;
    }

    uint32_t ip_address() override {
        return 0;
    }

    void service(Pool *pool) override {
    }

    PoolPointer<NetworkConnection> *open_connection(const char *scheme, const char *hostname, uint16_t port) override {
        return nullptr;
    }

    bool stop() override {
        return true;
    }

    bool enabled() override {
        return enabled_;
    }
};

template<typename T>
tl::expected<T, Error> as_expected(T e) {
    return tl::expected<T, Error>(e);
}

}
