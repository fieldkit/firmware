#include "printf.h"
#include "hal/linux/linux.h"
#include "hal/linux/linux_network.h"

#if !defined(ARDUINO)

#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

namespace fk {

FK_DECLARE_LOGGER("network");

LinuxNetworkConnection::LinuxNetworkConnection() {
}

LinuxNetworkConnection::LinuxNetworkConnection(int32_t s) : s_(s) {
}

LinuxNetworkConnection::~LinuxNetworkConnection() {
}

NetworkConnectionStatus LinuxNetworkConnection::status() {
    return NetworkConnectionStatus::Connected;
}

bool LinuxNetworkConnection::available() {
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
    auto needed = fk_vsnprintf(nullptr, 0, str, copy);
    auto buffer = (char *)malloc(needed + 1);
    fk_vsnprintf(buffer, needed + 1, str, args);
    write(buffer);
    free(buffer);
    return needed;
}

int32_t LinuxNetworkConnection::socket() {
    return s_;
}

bool LinuxNetworkConnection::stop() {
    if (s_ > 0) {
        close(s_);
        s_ = -1;
    }
    return true;
}

bool LinuxNetwork::begin(NetworkSettings settings) {
    settings_ = settings;
    enabled_ = true;

    return true;
}

bool LinuxNetwork::serve() {
    strncpy(service_name_, settings_.name, sizeof(service_name_) - 5);
    auto n = std::min(strlen(service_name_), sizeof(service_name_) - 5);
    strncpy(service_name_ + n, "._fk", 4);
    service_name_[n + 4] = 0;

    listening_ = socket(AF_INET, SOCK_STREAM, 0);
    if (listening_ == -1) {
        logerror("linux: unable to listen");
        return false;
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(2380);

    int32_t option = 1;
    setsockopt(listening_, SOL_SOCKET, SO_REUSEADDR, (char *)&option, sizeof(option));

    if (bind(listening_, (struct sockaddr *)&server, sizeof(server)) < 0) {
        logerror("linux: unable to bind to address");
        return false;
    }

    listen(listening_, 10);

    return true;
}

NetworkStatus LinuxNetwork::status() {
    return NetworkStatus::Listening;
}

uint32_t LinuxNetwork::ip_address() {
    return 0;
}

NetworkConnection *LinuxNetwork::accept() {
    fd_set rfd;
    FD_ZERO(&rfd);
    FD_SET(listening_, &rfd);

    struct timeval tov;
    tov.tv_sec = 0;
    tov.tv_usec = 10000;

    auto tv = select(listening_ + 1, &rfd, NULL, NULL, &tov);
    if (tv <= 0) {
        return nullptr;
    }

    struct sockaddr_in claddr;
    socklen_t c = sizeof(struct sockaddr_in);
    auto s = ::accept(listening_, (struct sockaddr *)&claddr, (socklen_t *)&c);
    if (s < 0) {
        return nullptr;
    }

    return new LinuxNetworkConnection(s);
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

}

#endif
