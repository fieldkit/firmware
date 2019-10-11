#include <loading.h>

#include "readings_taker.h"

#include "utilities.h"
#include "hal/hal.h"
#include "clock.h"
#include "records.h"
#include "state_ref.h"
#include "readings.h"
#include "storage/meta_ops.h"
#include "modules/module_factory.h"

extern const struct fkb_header_t fkb_header;

namespace fk {

FK_DECLARE_LOGGER("readings");

ReadingsTaker::ReadingsTaker(ModuleScanning &scanning, Storage &storage, ModMux *mm, bool read_only)
    : scanning_(scanning), storage_(storage), readings_{ mm }, mm_(mm), read_only_(read_only) {
}

tl::expected<TakenReadings, Error> ReadingsTaker::take(ScanningContext &ctx, Pool &pool) {
    auto modules = get_module_factory().create(scanning_, ctx, pool);
    if (!modules) {
        return tl::unexpected<Error>(modules.error());
    }

    if ((*modules).size() == 0) {
        loginfo("no modules");
        return TakenReadings{ 0, { } };
    }

    if (!read_only_) {
        auto meta_record = append_configuration(*modules, pool);
        if (!meta_record) {
            logerror("error appending configuration");
            return tl::unexpected<Error>(Error::General);
        }

        auto data = storage_.file(Storage::Data);
        if (!data.seek_end()) {
            FK_ASSERT(data.create());
        }

        auto number = data.record();
        auto all_readings = readings_.take_readings(ctx, *modules, *meta_record, number, pool);
        if (!all_readings) {
            logerror("error taking readings");
            return tl::unexpected<Error>(Error::General);
        }

        if (!append_readings(data, pool)) {
            logerror("error appending readings");
            return tl::unexpected<Error>(Error::General);
        }

        if (!verify_reading_record(data, pool)) {
            logerror("error verifying readings");
            return tl::unexpected<Error>(Error::General);
        }

        return TakenReadings{ number, *all_readings };
    }

    auto all_readings = readings_.take_readings(ctx, *modules, 0, 0, pool);
    if (!all_readings) {
        logerror("error taking readings");
        return tl::unexpected<Error>(Error::General);
    }

    return TakenReadings{ 0, *all_readings };
}

bool ReadingsTaker::append_readings(File &file, Pool &pool) {
    for (size_t i = 0; i < fk_config().readings.amplification; ++i) {
        if (i > 0) {
            // NOTE: This is necessary when we're amplifying.
            readings_.record().readings.reading++;
        }
        auto bytes_wrote = file.write(&readings_.record(), fk_data_DataRecord_fields);
        if (bytes_wrote == 0) {
            logerror("error saving readings");
            return false;
        }

        auto record = readings_.record().readings.reading;
        loginfo("wrote %zd bytes file=(#%" PRIu32 ") rec=(#%" PRIu32 ") (%" PRIu32 " bytes) (" PRADDRESS ")",
                (size_t)bytes_wrote, file.previous_record(), record, file.size(), file.tail());
    }

    return true;
}

bool ReadingsTaker::verify_reading_record(File &file, Pool &pool) {
    if (!file.seek_end()) {
        return false;
    }

    auto &record = readings_.record();

    if (file.previous_record() != record.readings.reading) {
        logerror("unexpected record (%" PRIu32 " != %" PRIu32 ")", file.previous_record(), record.readings.reading);
        return false;
    }

    return true;
}

tl::expected<uint32_t, Error> ReadingsTaker::append_configuration(ConstructedModulesCollection &modules, Pool &pool) {
    MetaOps ops{ storage_ };
    auto gs = get_global_state_ro();
    auto modules_record = ops.write_modules(gs.get(), modules, pool);
    return modules_record;
}

}
