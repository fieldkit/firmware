#include <SEGGER_RTT.h>

#include "debug_connection.h"

namespace fk {

constexpr size_t BufferSize = 1024;

DebugServerConnection::DebugServerConnection(Pool *pool, NetworkConnection *conn, uint32_t number) : Connection(pool, conn, number), writer_(this, (uint8_t *)pool->malloc(BufferSize), BufferSize) {
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

    for (auto c : lb) {
        writer_.write(c);
    }

    writer_.flush();

    lb.zero();

    return true;
}

}
