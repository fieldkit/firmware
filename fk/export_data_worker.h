#pragma once

#include "worker.h"
#include "storage/storage.h"
#include "storage/meta_record.h"

#include "hal/sd_card.h"

namespace fk {

class ExportDataWorker : public Worker {
private:
    StandardPool meta_pool_{ "meta-pool" };
    MetaRecord meta_record_{ meta_pool_ };
    DataMemory *data_memory_;
    TaskDisplayInfo info_;
    uint32_t meta_record_number_{ InvalidRecord };
    SdCardFile *writing_{ nullptr };

public:
    explicit ExportDataWorker();
    explicit ExportDataWorker(DataMemory *data_memory);

public:
    void run(Pool &pool) override;

private:
    bool lookup_meta(uint32_t meta_record_number, FileReader *meta_file, Pool &pool);
    bool write_header();

    enum WriteStatus {
        Success,
        Debug,
        Fatal
    };

    WriteStatus write_row(fk_data_DataRecord &record);

public:
    const char *name() const override {
        return "expdata";
    }

    TaskDisplayInfo display_info() const override {
        return info_;
    }

};

FK_ENABLE_TYPE_NAME(ExportDataWorker);

} // namespace fk
