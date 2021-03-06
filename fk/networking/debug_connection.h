#pragma once

#include "networking/connection.h"

namespace fk {

class DebugServerConnection : public Connection {
private:
    BufferedWriter writer_;
    log_buffer::iterator iter_;

public:
    DebugServerConnection(Pool *pool, NetworkConnection *conn, uint32_t number);
    virtual ~DebugServerConnection();

public:
    bool service() override;

};

}
