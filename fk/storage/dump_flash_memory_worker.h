#pragma once

#include "worker.h"
#include "storage/storage.h"
#include "hal/sd_card.h"

namespace fk {

class DumpFlashMemoryWorker : public Worker {
public:
    void run(Pool &pool) override;

public:
    const char *name() const override {
        return "dumpmem";
    }

private:
    void dump_darwin_storage(SdCardFile *file, Pool &pool);
    void dump_phylum_storage(SdCardFile *file, Pool &pool);
};

FK_ENABLE_TYPE_NAME(DumpFlashMemoryWorker);

} // namespace fk
