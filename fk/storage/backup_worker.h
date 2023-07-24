#pragma once

#include "worker.h"
#include "storage/storage.h"
#include "storage/meta_record.h"
#include "hal/sd_card.h"

namespace fk {

class BackupWorker : public Worker {
private:
    bool verify_{ false };
    TaskDisplayInfo info_;

public:
    explicit BackupWorker();

public:
    void run(Pool &pool) override;

public:
    const char *name() const override {
        return "backup";
    }

    TaskDisplayInfo display_info() const override {
        return info_;
    }

private:
    bool write_file(FileReader *file, const char *path, Pool &pool);
    bool hash_file(const char *path, uint8_t *hash, Pool &pool);
};

FK_ENABLE_TYPE_NAME(BackupWorker);

} // namespace fk
