#pragma once

#include "worker.h"
#include "storage/storage.h"
#include "storage/meta_record.h"

namespace fk {

class ExportDataWorker : public Worker {
private:
    StandardPool meta_pool_{ "meta-pool" };
    DataMemory *data_memory_;
    TaskDisplayInfo info_;
    MetaRecord meta_record_;
    uint32_t meta_record_number_{ InvalidRecord };

public:
    ExportDataWorker();
    ExportDataWorker(DataMemory *data_memory);

public:
    void run(Pool &pool) override;

private:
    bool lookup_meta(uint32_t meta_record_number, File &meta_file, Pool &pool);

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
