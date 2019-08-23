#include <loading.h>

#include "readings_taker.h"

#include "hal/hal.h"
#include "module_factory.h"
#include "clock.h"
#include "protobuf.h"
#include "storage/signed_log.h"
#include "state.h"
#include "utilities.h"
#include "readings.h"

extern const struct fkb_header_t fkb_header;

namespace fk {

FK_DECLARE_LOGGER("readings");

static bool append_readings(Readings &readings, File &file, Pool &pool);

static bool append_configuration(ModuleContext &mc, ConstructedModulesCollection &modules, File &file, Pool &pool);

ReadingsTaker::ReadingsTaker(ModuleScanning &scanning, Storage &storage) : scanning_(scanning), storage_(storage) {
}

bool ReadingsTaker::take(ModuleContext &mc, Pool &pool) {
    auto data = storage_.file(Storage::Data);
    auto meta = storage_.file(Storage::Meta);

    ModuleFactory module_factory(scanning_, &pool);
    auto modules = module_factory.create();
    if (!modules) {
        return false;
    }

    if ((*modules).size() == 0) {
        loginfo("no modules");
        return true;
    }

    auto mm = get_modmux();

    loginfo("initializing modules");

    for (auto pair : *modules) {
        auto module = pair.module;
        auto i = pair.found.position;

        if (i != 0xff) {
            if (!mm->choose(i)) {
                logerror("error choosing module");
                return false;
            }
        }

        if (!module->initialize(mc, pool)) {
            logerror("error initializing module");
            return false;
        }
    }

    if (!append_configuration(mc, *modules, meta, pool)) {
        logerror("error appending configuration");
        return false;
    }

    Readings readings{ mm };
    if (!readings.take_readings(mc, *modules, data.record(), pool)) {
        logerror("error taking readings");
        return false;
    }

    if (!append_readings(readings, data, pool)) {
        logerror("error appending readings");
        return false;
    }

    return true;
}

static bool append_readings(Readings &readings, File &file, Pool &pool) {
    for (auto i = 0; i < FK_READINGS_AMPLIFY_WRITES; ++i) {
        auto bytes_wrote = file.write(&readings.record(), fk_data_DataRecord_fields);
        if (bytes_wrote == 0) {
            logerror("error saving readings");
            return false;
        }

        loginfo("wrote %zd bytes (#%" PRIu32 ") (%" PRIu32 " bytes) (" PRADDRESS ")",
                bytes_wrote, file.record() - 1, file.size(), file.tail());
    }

    return true;
}

static bool append_configuration(ModuleContext &mc, ConstructedModulesCollection &modules, File &file, Pool &pool) {
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

    fk_data_DataRecord record = fk_data_DataRecord_init_default;
    record.metadata.firmware.git.funcs.encode = pb_encode_string;
    record.metadata.firmware.git.arg = (void *)hash_hex;
    record.metadata.firmware.build.funcs.encode = pb_encode_string;
    record.metadata.firmware.build.arg = (void *)fkb_header.firmware.name;
    record.metadata.deviceId.funcs.encode = pb_encode_data;
    record.metadata.deviceId.arg = (void *)&device_id;
    record.modules.funcs.encode = pb_encode_array;
    record.modules.arg = (void *)modules_array;

    SignedRecordLog srl{ file };

    if (!srl.append_immutable(SignedRecordKind::Modules, &record, fk_data_DataRecord_fields, pool)) {
        return false;
    }

    return true;
}

}
