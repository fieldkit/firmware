#include <loading.h>

#include "readings_taker.h"

#include "utilities.h"
#include "hal/hal.h"
#include "clock.h"
#include "records.h"
#include "state.h"
#include "readings.h"
#include "storage/signed_log.h"
#include "modules/module_factory.h"

extern const struct fkb_header_t fkb_header;

namespace fk {

FK_DECLARE_LOGGER("readings");

ReadingsTaker::ReadingsTaker(ModuleScanning &scanning, Storage &storage, ModMux *mm, bool read_only)
    : scanning_(scanning), storage_(storage), readings_{ mm }, mm_(mm), read_only_(read_only) {
}

tl::expected<ModuleReadingsCollection, Error> ReadingsTaker::take(ScanningContext &ctx, Pool &pool) {
    auto modules = get_module_factory().create(scanning_, ctx, pool);
    if (!modules) {
        return tl::unexpected<Error>(modules.error());
    }

    if ((*modules).size() == 0) {
        loginfo("no modules");
        return ModuleReadingsCollection{ };
    }

    if (!read_only_) {
        auto meta = storage_.file(Storage::Meta);
        if (!meta.seek_end()) {
            FK_ASSERT(meta.create());
        }

        if (!append_configuration(ctx, *modules, meta, pool)) {
            logerror("error appending configuration");
            return tl::unexpected<Error>(Error::General);
        }

        auto data = storage_.file(Storage::Data);
        if (!data.seek_end()) {
            FK_ASSERT(data.create());
        }

        auto all_readings = readings_.take_readings(ctx, *modules, data.record(), pool);
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

        return all_readings;
    }

    auto all_readings = readings_.take_readings(ctx, *modules, 0, pool);
    if (!all_readings) {
        logerror("error taking readings");
        return tl::unexpected<Error>(Error::General);
    }

    return all_readings;
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

bool ReadingsTaker::append_configuration(ScanningContext &ctx, ConstructedModulesCollection &modules, File &file, Pool &pool) {
    auto module_infos = pool.malloc<fk_data_ModuleInfo>(modules.size());

    auto index = 0;
    for (auto pair : modules) {
        auto meta = pair.meta;
        auto module = pair.module;
        auto sensor_metas = module->get_sensors(ctx.module(0), pool);

        auto id_data = pool.malloc_with<pb_data_t>({
            .length = sizeof(fk_uuid_t),
            .buffer = &pair.found.header.id,
        });


        auto &m = module_infos[index];
        m = fk_data_ModuleInfo_init_default;
        m.position = index;
        m.id.funcs.encode = pb_encode_data;
        m.id.arg = (void *)id_data;
        m.name.funcs.encode = pb_encode_string;
        m.name.arg = (void *)meta->name;
        m.header.manufacturer = meta->manufacturer;
        m.header.kind = meta->kind;
        m.header.version = meta->version;

        auto sensor_infos = pool.malloc<fk_data_SensorInfo>(sensor_metas->nsensors);
        for (size_t i = 0; i < sensor_metas->nsensors; ++i) {
            sensor_infos[i] = fk_data_SensorInfo_init_default;
            sensor_infos[i].name.funcs.encode = pb_encode_string;
            sensor_infos[i].name.arg = (void *)sensor_metas->sensors[i].name;
            sensor_infos[i].unitOfMeasure.funcs.encode = pb_encode_string;
            sensor_infos[i].unitOfMeasure.arg = (void *)sensor_metas->sensors[i].unitOfMeasure;
        }

        auto sensors_array = pool.malloc_with<pb_array_t>({
            .length = sensor_metas->nsensors,
            .itemSize = sizeof(fk_data_SensorInfo),
            .buffer = sensor_infos,
            .fields = fk_data_SensorInfo_fields,
        });

        m.sensors.funcs.encode = pb_encode_array;
        m.sensors.arg = (void *)sensors_array;

        index++;
    }

    auto modules_array = pool.malloc_with<pb_array_t>({
        .length = (size_t)modules.size(),
        .itemSize = sizeof(fk_data_ModuleInfo),
        .buffer = module_infos,
        .fields = fk_data_ModuleInfo_fields,
    });

    fk_serial_number_t sn;
    fk_serial_number_get(&sn);

    pb_data_t device_id = {
        .length = sizeof(sn),
        .buffer = &sn,
    };

    auto hash_size = fkb_header.firmware.hash_size;
    auto hash_hex = bytes_to_hex_string_pool(fkb_header.firmware.hash, hash_size, pool);

    auto record = fk_data_record_encoding_new();
    record.metadata.firmware.git.arg = (void *)hash_hex;
    record.metadata.firmware.build.arg = (void *)fkb_header.firmware.name;
    record.metadata.deviceId.arg = (void *)&device_id;
    record.modules.arg = (void *)modules_array;

    auto srl = SignedRecordLog { file };
    if (!srl.append_immutable(SignedRecordKind::Modules, &record, fk_data_DataRecord_fields, pool)) {
        return false;
    }

    return true;
}

}
