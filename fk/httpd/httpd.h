#pragma once

#include "../common.h"

namespace fk {

enum class ConnectionHandling : uint8_t {
    Close,
    KeepAlive
};

enum class TransferEncoding : uint8_t {
    Default,
    Chunked,
};

class Server {
private:

public:

public:

};

class Request {
private:
    const char *path;
    uint32_t length_;
    ConnectionHandling connection_;
    TransferEncoding encoding_;

public:
    Request(uint32_t fd);

};

class Response {
private:

public:
    Response(uint32_t fd);

public:

};

}
