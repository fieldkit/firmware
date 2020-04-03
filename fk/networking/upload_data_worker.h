#pragma once

#include "worker.h"
#include "storage/storage.h"

namespace fk {

class UploadDataWorker : public Worker {
private:
    bool all_meta_{ false };
    bool all_data_{ false };

public:
    UploadDataWorker();
    UploadDataWorker(bool all_meta, bool all_data);

public:
    void run(Pool &pool) override;

public:
    uint8_t priority() const override {
        return OS_PRIORITY_NORMAL + 1;
    }

    const char *name() const override {
        return "upldata";
    }

private:
    struct FileUpload {
        uint32_t record;

        FileUpload(uint32_t record) : record(record) {
        }

        operator bool() {
            return record > 0;
        }
    };

    FileUpload upload_file(Storage &storage, uint8_t file_number, uint32_t first_record, const char *type, Pool &pool);

};

FK_ENABLE_TYPE_NAME(UploadDataWorker);

}
