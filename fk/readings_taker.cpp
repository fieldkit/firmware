#include <loading.h>

#include "readings_taker.h"

#include "hal/hal.h"
#include "module_factory.h"
#include "clock.h"
#include "records.h"
#include "storage/signed_log.h"
#include "state.h"
#include "utilities.h"
#include "readings.h"

extern const struct fkb_header_t fkb_header;

namespace fk {

FK_DECLARE_LOGGER("readings");

ReadingsTaker::ReadingsTaker(ModuleScanning &scanning, Storage &storage, ModMux *mm) : scanning_(scanning), storage_(storage), readings_{ mm }, mm_(mm) {
}

bool ReadingsTaker::take(ModuleContext &mc, Pool &pool) {
    ModuleFactory module_factory(scanning_, &pool);
    auto modules = module_factory.create();
    if (!modules) {
        return false;
    }

    if ((*modules).size() == 0) {
        loginfo("no modules");
        return true;
    }

    if (!initialize_modules(mc, *modules, mm_, pool)) {
        return false;
    }

    auto meta = storage_.file(Storage::Meta);
    if (!meta.seek_end()) {
        FK_ASSERT(meta.create());
    }

    if (!append_configuration(mc, *modules, meta, pool)) {
        logerror("error appending configuration");
        return false;
    }

    auto data = storage_.file(Storage::Data);
    if (!data.seek_end()) {
        FK_ASSERT(data.create());
    }

    if (!readings_.take_readings(mc, *modules, data.record(), pool)) {
        logerror("error taking readings");
        return false;
    }


    if (!append_readings(data, pool)) {
        logerror("error appending readings");
        return false;
    }

    if (!verify_reading_record(data, pool)) {
        return false;
    }

    return true;
}

bool ReadingsTaker::initialize_modules(ModuleContext &mc, ConstructedModulesCollection &modules, ModMux *mm, Pool &pool) {
    loginfo("initializing modules");

    for (auto pair : modules) {
        auto module = pair.module;

        if (!mm->choose(pair.found.position)) {
            logerror("error choosing module");
            return false;
        }

        if (!module->initialize(mc, pool)) {
            logerror("error initializing module");
            return false;
        }
    }

    return true;
}

bool ReadingsTaker::append_readings(File &file, Pool &pool) {
    for (auto i = 0; i < FK_READINGS_AMPLIFY_WRITES; ++i) {
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
                bytes_wrote, file.previous_record(), record, file.size(), file.tail());
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

bool ReadingsTaker::append_configuration(ModuleContext &mc, ConstructedModulesCollection &modules, File &file, Pool &pool) {
    auto module_infos = pool.malloc<fk_data_ModuleInfo>(modules.size());

    auto index = 0;
    for (auto pair : modules) {
        auto meta = pair.meta;
        auto module = pair.module;
        auto sensor_metas = module->get_sensors(mc, pool);

        auto &m = module_infos[index];
        m = fk_data_ModuleInfo_init_default;
        m.id = index;
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
