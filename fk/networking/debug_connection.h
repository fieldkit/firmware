#pragma once

#include "networking/connection.h"

namespace fk {

class DebugServerConnection : public Connection {
private:

public:
    DebugServerConnection(Pool *pool, NetworkConnection *conn, uint32_t number);
    virtual ~DebugServerConnection();

};

}
