#include <loading.h>

#include "readings_taker.h"

#include "utilities.h"
#include "hal/hal.h"
#include "clock.h"
#include "records.h"
#include "state_ref.h"
#include "readings.h"
#include "modules/module_factory.h"

extern const struct fkb_header_t fkb_header;

namespace fk {

FK_DECLARE_LOGGER("readings");

ReadingsTaker::ReadingsTaker(Storage &storage, ModMux *mm, bool read_only)
    : storage_(storage), readings_{ mm }, mm_(mm), read_only_(read_only) {
}

tl::expected<TakenReadings, Error> ReadingsTaker::take(ConstructedModulesCollection &constructed_modules, ScanningContext &ctx, Pool &pool) {
    if (constructed_modules.size() == 0) {
        loginfo("no modules");
        return TakenReadings{ 0, 0, { pool }, { pool } };
    }

    if (!read_only_) {
        auto all_readings = readings_.take_readings(ctx, constructed_modules, pool);
        if (!all_readings) {
            logerror("taking readings");
            return tl::unexpected<Error>(Error::General);
        }

        auto meta_record = append_configuration(constructed_modules, *all_readings, pool);
        if (!meta_record) {
            logerror("appending configuration");
            return tl::unexpected<Error>(Error::General);
        }

        auto data_record = append_readings(*meta_record, pool);
        if (!data_record) {
            logerror("appending readings");
            return tl::unexpected<Error>(Error::General);
        }

        if (!storage_.flush()) {
            logerror("flushing");
            return tl::unexpected<Error>(Error::IO);
        }

        return TakenReadings{ get_clock_now(), *data_record, std::move(constructed_modules), std::move(*all_readings) };
    }

    auto all_readings = readings_.take_readings(ctx, constructed_modules, pool);
    if (!all_readings) {
        logerror("taking readings");
        return tl::unexpected<Error>(Error::General);
    }

    return TakenReadings{ 0, 0, std::move(constructed_modules), std::move(*all_readings) };
}

tl::expected<uint32_t, Error> ReadingsTaker::append_configuration(ConstructedModulesCollection &modules, ModuleReadingsCollection &readings, Pool &pool) {
    auto gs = get_global_state_rw();
    return storage_.meta_ops()->write_modules(gs.get(), &fkb_header, modules, readings, pool);
}

tl::expected<uint32_t, Error> ReadingsTaker::append_readings(uint32_t meta_record, Pool &pool) {
    // We fill in the number in write_readings
    readings_.record().readings.meta = meta_record;

    auto gs = get_global_state_rw();
    return storage_.data_ops()->write_readings(gs.get(), &readings_.record(), pool);
}

}
