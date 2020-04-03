#include "export_data_worker.h"
#include "hal/hal.h"
#include "hal/memory.h"
#include "hal/sd_card.h"
#include "utilities.h"
#include "clock.h"
#include "state_manager.h"

namespace fk {

FK_DECLARE_LOGGER("expdata");

void ExportDataWorker::run(Pool &pool) {
    auto lock = storage_mutex.acquire(UINT32_MAX);
    StatisticsMemory memory{ MemoryFactory::get_data_memory() };
    Storage storage{ &memory, pool };
    auto sd = get_sd_card();

    FormattedTime formatted{ get_clock_now(), TimeFormatMachine };
    auto path = pool.sprintf("/%s/data.csv", formatted.cstr());

    if (!sd->begin()) {
        logerror("error opening sd card");
        return;
    }

    if (!sd->mkdir(formatted.cstr())) {
        logerror("error making directory '%s'", formatted.cstr());
        return;
    }

    auto file = sd->open(path, true, pool);
    if (file == nullptr || !file) {
        logerror("unable to open '%s'", path);
        return;
    }

    if (!file->close()) {
        logerror("error closing");
        return;
    }
}

} // namespace fk
