#pragma once

#include "worker.h"
#include "storage/storage.h"

namespace fk {

class ExportDataWorker : public Worker {
private:
    DataMemory *data_memory_;
    TaskDisplayInfo info_;

public:
    ExportDataWorker();
    ExportDataWorker(DataMemory *data_memory);

public:
    void run(Pool &pool) override;

public:
    uint8_t priority() const override {
        return OS_PRIORITY_NORMAL + 1;
    }

    const char *name() const override {
        return "expdata";
    }

    TaskDisplayInfo display_info() const override {
        return info_;
    }

};

FK_ENABLE_TYPE_NAME(ExportDataWorker);

} // namespace fk
