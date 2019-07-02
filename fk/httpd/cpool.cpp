#include <fk-app-protocol.h>

#include "cpool.h"

namespace fk {

#define loginfo(f, ...)  loginfof("httpd", f, ##__VA_ARGS__)

#define logerror(f, ...) logerrorf("httpd", f, ##__VA_ARGS__)

ConnectionPool::ConnectionPool() : memory_("httpd", 0 * HttpdConnectionWorkSize * MaximumConnections) {
}

size_t ConnectionPool::available() {
    size_t used = 0;
    for (auto i = (size_t)0; i < MaximumConnections; ++i) {
        if (pool_[i] != nullptr) {
            used++;
        }
    }
    return MaximumConnections - used;
}

void ConnectionPool::service() {
    for (auto i = (size_t)0; i < MaximumConnections; ++i) {
        if (pool_[i] != nullptr) {
            if (!pool_[i]->service()) {
                delete pool_[i];
                pool_[i] = nullptr;
            }
        }
    }
}

void ConnectionPool::queue(WifiConnection *c) {
    for (auto i = (size_t)0; i < MaximumConnections; ++i) {
        if (pool_[i] == nullptr) {
            pool_[i] = new Connection(c, HttpdConnectionWorkSize);
            return;
        }
    }
    FK_ASSERT(false);
}

Connection::Connection(WifiConnection *conn, size_t size) : conn_(conn), pool_{ "conn", size }, req_{ &pool_ }, buffer_{ nullptr }, size_{ 0 }, position_{ 0 } {
}

Connection::~Connection() {
    conn_->stop();
}

bool Connection::service() {
    // TODO: 414 Request-URI Too Long
    // TODO: 431 Request Header Fields Too Large.
    // TODO: 413 Payload Too Large
    // TODO: 500 Service Unavailable
    // TODO: 503 Service Unavailable
    if (conn_->status() != WifiConnectionStatus::Connected) {
        loginfo("disconnected");
        return false;
    }

    if (conn_->available()) {
        if (buffer_ == nullptr) {
            size_ = HttpdConnectionBufferSize;
            buffer_ = (uint8_t *)pool_.malloc(HttpdConnectionBufferSize);
            position_ = 0;
        }

        auto available = (size_ - 1) - position_;
        auto nread = conn_->read(buffer_ + position_, available);
        if (nread < 0) {
            loginfo("error reading - eos");
            return false;
        }

        if (nread == 0) {
            loginfo("error reading - empty");
            return false;
        }

        auto ptr = (char *)(buffer_ + position_);
        ptr[nread] = 0;
        if (req_.parse(ptr, nread) != 0) {
            loginfo("error parsing");
            return false;
        }

        position_ += nread;
    }

    if (req_.have_headers()) {
        loginfo("routing '%s' (%" PRIu32 " bytes)", req_.url(), req_.length());
    }

    if (req_.consumed()) {
        loginfo("replying/closing");
        conn_->write("HTTP/1.1 204 OK\n");
        conn_->write("Content-Length: 0\n");
        conn_->write("Content-Type: text/html\n");
        conn_->write("Connection: close\n");
        conn_->write("\n");
        return false;
    }

    return true;
}

}
