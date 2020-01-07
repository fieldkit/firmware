#include <SEGGER_RTT.h>

#include "debug_connection.h"

namespace fk {

DebugServerConnection::DebugServerConnection(Pool *pool, NetworkConnection *conn, uint32_t number) : Connection(pool, conn, number), writer_(this, (uint8_t *)pool->malloc(NetworkBufferSize), NetworkBufferSize) {
}

DebugServerConnection::~DebugServerConnection() {
}

bool DebugServerConnection::service() {
    if (!Connection::service()) {
        return false;
    }

    auto &lb = fk_log_buffer();

    if (lb.empty()) {
        return true;
    }

    LogBufferLock lock;

    for (auto c : lb) {
        writer_.write(c);
    }

    writer_.flush();

    return true;
}

}
