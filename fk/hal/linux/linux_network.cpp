#include <tiny_printf.h>

#include "hal/linux/linux.h"
#include "hal/linux/linux_network.h"

#if !defined(__SAMD51__)

#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

namespace fk {

FK_DECLARE_LOGGER("network");

LinuxNetworkConnection::LinuxNetworkConnection() {
}

LinuxNetworkConnection::LinuxNetworkConnection(Pool *pool, int32_t s, uint32_t remote_address) : s_(s), remote_address_(remote_address) {
}

LinuxNetworkConnection::~LinuxNetworkConnection() {
}

NetworkConnectionStatus LinuxNetworkConnection::status() {
    if (s_ == -1) {
        return NetworkConnectionStatus::Disconnected;
    }
    return NetworkConnectionStatus::Connected;
}

bool LinuxNetworkConnection::available() {
    if (s_ == -1) {
        return false;
    }

    fd_set rfd;
    FD_ZERO(&rfd);
    FD_SET(s_, &rfd);

    struct timeval tov;
    tov.tv_sec = 0;
    tov.tv_usec = 10000;

    auto tv = select(s_ + 1, &rfd, NULL, NULL, &tov);
    if (tv <= 0) {
        return false;
    }

    return true;
}

int32_t LinuxNetworkConnection::read(uint8_t *buffer, size_t size) {
    return ::read(s_, buffer, size);
}

int32_t LinuxNetworkConnection::write(const char *str) {
    return ::write(s_, str, strlen(str));
}

int32_t LinuxNetworkConnection::write(const uint8_t *buffer, size_t size) {
    return ::write(s_, buffer, size);
}

int32_t LinuxNetworkConnection::writef(const char *str, ...) {
    va_list args;
    va_start(args, str);
    auto rv = vwritef(str, args);
    va_end(args);
    return rv;
}

int32_t LinuxNetworkConnection::vwritef(const char *str, va_list args) {
    va_list copy;
    va_copy(copy, args);
    auto needed = tiny_vsnprintf(nullptr, 0, str, copy);
    auto buffer = (char *)fk_malloc(needed + 1);
    tiny_vsnprintf(buffer, needed + 1, str, args);
    write(buffer);
    fk_free(buffer);
    va_end(copy);
    return needed;
}

uint32_t LinuxNetworkConnection::remote_address() {
    return remote_address_;
}

bool LinuxNetworkConnection::stop() {
    if (s_ > 0) {
        close(s_);
        s_ = -1;
    }
    return true;
}

bool LinuxNetwork::begin(NetworkSettings settings, Pool *pool) {
    settings_ = settings;
    enabled_ = true;

    return true;
}

bool LinuxNetwork::serve() {
    return true;
}

NetworkStatus LinuxNetwork::status() {
    return NetworkStatus::Connected;
}

uint32_t LinuxNetwork::ip_address() {
    return 0;
}

PoolPointer<NetworkListener> *LinuxNetwork::listen(uint16_t port) {
    auto linux_port = port + 2300;

    auto listening = ::socket(AF_INET, SOCK_STREAM, 0);
    if (listening == -1) {
        logerror("linux: unable to listen");
        return nullptr;
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(linux_port);

    int32_t option = 1;
    ::setsockopt(listening, SOL_SOCKET, SO_REUSEADDR, (char *)&option, sizeof(option));

    if (::bind(listening, (struct sockaddr *)&server, sizeof(server)) < 0) {
        logerror("linux: unable to bind to address (%d)", linux_port);
        return nullptr;
    }

    ::listen(listening, 10);

    loginfo("linux: listening on %d", linux_port);

    return create_network_listener_wrapper<LinuxNetworkListener>(port, listening);
}

void LinuxNetwork::service(Pool *pool) {
}

PoolPointer<NetworkConnection> *LinuxNetwork::open_connection(const char *scheme, const char *hostname, uint16_t port) {
    return nullptr;
}

bool LinuxNetwork::stop() {
    if (enabled_) {
        enabled_ = false;
        close(listening_);
        listening_ = -1;
    }
    return true;
}

bool LinuxNetwork::enabled() {
    return enabled_;
}

bool LinuxNetwork::synchronize_time() {
    return true;
}

bool LinuxNetwork::get_mac_address(uint8_t *address) {
    for (auto i = 0u; i < 6; ++i) {
        address[i] = 0;
    }
    return true;
}

const char *LinuxNetwork::get_ssid() {
    return "";
}

bool LinuxNetwork::get_created_ap() {
    return false;
}

NetworkScan LinuxNetwork::scan(Pool &pool) {
    return { nullptr, 0 };
}

void LinuxNetwork::verify() {
}

LinuxNetworkListener::LinuxNetworkListener(Pool *pool, uint16_t port, int32_t listening) : port_(port), listening_(listening) {
}

PoolPointer<NetworkConnection> *LinuxNetworkListener::accept() {
    fd_set rfd;
    FD_ZERO(&rfd);
    FD_SET(listening_, &rfd);

    struct timeval tov;
    tov.tv_sec = 0;
    tov.tv_usec = 10000;

    auto tv = ::select(listening_ + 1, &rfd, NULL, NULL, &tov);
    if (tv <= 0) {
        return nullptr;
    }

    struct sockaddr_in claddr;
    socklen_t c = sizeof(struct sockaddr_in);
    auto s = ::accept(listening_, (struct sockaddr *)&claddr, (socklen_t *)&c);
    if (s < 0) {
        return nullptr;
    }

    return create_network_connection_wrapper<LinuxNetworkConnection>(s, claddr.sin_addr.s_addr);
}

bool LinuxNetworkListener::stop() {
    if (listening_ > 0) {
        ::close(listening_);
        listening_ = -1;
    }

    return true;
}

} // namespace fk

#endif
