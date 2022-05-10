#pragma once

#include "networking/networking.h"
#include "worker.h"

namespace fk {

class WriteMemoryHandler : public HttpHandler {
public:
    bool handle(HttpServerConnection *connection, Pool &pool) override;
};

class WriteMemoryWorker : public Worker {
private:
    HttpServerConnection *connection_;

public:
    WriteMemoryWorker(HttpServerConnection *connection);

public:
    void run(Pool &pool) override;

    const char *name() const override {
        return "writemem";
    }

private:
    void serve(Pool &pool);

    /**
     * This is dumb and only here to workaround a common bug in HTTP
     * client libraries that will EPIPE/Broken Pipe if we reply with
     * error headers before consuming the entire body. Would love to remove.
     */
    bool read_complete_and_fail(const char *error, Pool &pool);
    bool write_error(const char *kind, Pool &pool);
    bool write_success(Pool &pool);
};

FK_ENABLE_TYPE_NAME(WriteMemoryWorker);

} // namespace fk
