#pragma once

#include "worker.h"
#include "storage/storage.h"
#include "storage/meta_record.h"
#include "hal/sd_card.h"

namespace fk {

class BackupWorker : public Worker {
public:
    explicit BackupWorker();

public:
    void run(Pool &pool) override;

public:
    uint8_t priority() const override {
        return OS_PRIORITY_NORMAL + 1;
    }

    const char *name() const override {
        return "backup";
    }

private:
    bool write_file(FileReader &file, const char *path, Pool &pool);
    bool hash_file(const char *path, uint8_t *hash, Pool &pool);

};

FK_ENABLE_TYPE_NAME(BackupWorker);

} // namespace fk
