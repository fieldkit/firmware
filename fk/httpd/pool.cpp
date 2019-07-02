#include "pool.h"

namespace fk {

#define loginfo(f, ...)  loginfof("httpd", f, ##__VA_ARGS__)

#define logerror(f, ...) logerrorf("httpd", f, ##__VA_ARGS__)

size_t ConnectionPool::available() {
    size_t used = 0;
    for (auto i = (size_t)0; i < MaximumConnections; ++i) {
        if (pool_[i].c != nullptr) {
            used++;
        }
    }
    return MaximumConnections - used;
}

void ConnectionPool::service() {
    for (auto i = (size_t)0; i < MaximumConnections; ++i) {
        if (pool_[i].c != nullptr) {
            if (!service(&pool_[i])) {
                // loginfo("%d removing", i);
                pool_[i].c->stop();
                delete pool_[i].c;
                pool_[i] = { };
            }
        }
    }
}

void ConnectionPool::queue(WifiConnection *c) {
    for (auto i = (size_t)0; i < MaximumConnections; ++i) {
        if (pool_[i].c == nullptr) {
            // loginfo("%d queued", i);
            pool_[i].c = c;
            pool_[i].position = 0;
            pool_[i].req.begin();
            return;
        }
    }
    FK_ASSERT(false);
}

bool ConnectionPool::service(Connection *c) {
    // TODO: 414 Request-URI Too Long
    // TODO: 431 Request Header Fields Too Large.
    // TODO: 413 Payload Too Large
    // TODO: 500 Service Unavailable
    // TODO: 503 Service Unavailable
    if (c->c->status() != WifiConnectionStatus::Connected) {
        return false;
    }

    if (c->c->available()) {
        auto available = (sizeof(c->data) - 1) - c->position;
        auto nread = c->c->read(c->data, available);
        if (nread < 0) {
            loginfo("EOS read");
            return false;
        }

        if (nread == 0) {
            loginfo("empty read");
            return false;
        }

        auto ptr = (char *)(c->data + c->position);
        ptr[nread] = 0;
        if (c->req.parse(ptr, nread) != 0) {
            return false;
        }

        c->position += nread;
    }

    if (c->req.have_headers()) {
        loginfo("routing '%s' (%" PRIu32 " bytes)", c->req.url(), c->req.length());
    }

    if (c->req.consumed()) {
        c->c->write("HTTP/1.1 204 OK\n");
        c->c->write("Content-Length: 0\n");
        c->c->write("Content-Type: text/html\n");
        c->c->write("Connection: close\n");
        c->c->write("\n");
        return false;
    }

    return true;
}

}
