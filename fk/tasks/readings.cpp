#include <loading.h>

#include <fk-data-protocol.h>

#include "tasks/tasks.h"
#include "hal/hal.h"
#include "hal/metal/metal.h"
#include "clock.h"
#include "protobuf.h"
#include "storage/storage.h"
#include "storage/signed_log.h"
#include "state.h"
#include "scanning.h"
#include "registry.h"
#include "readings.h"
#include "utilities.h"

extern const struct fkb_header_t fkb_header;

namespace fk {

FK_DECLARE_LOGGER("readings");

// Quick code to write a module EEPROM header for development purposes.
// Eventually we should have a secret menu or something for doing this kind
// of thing.
static void configure_module() __attribute__((unused));

static bool append_configuration(ModuleContext &mc, ResolvedModules &modules, File &file, Pool &pool);

static bool append_readings(Readings &readings, File &file, Pool &pool);

void task_handler_readings(void *params) {
    auto lock = storage_mutex.acquire(UINT32_MAX);
    auto started = fk_uptime();
    auto pool = MallocPool{ "readings", ModuleMemoryAreaSize };
    auto memory_bus = get_board()->spi_flash();
    auto module_bus = get_board()->i2c_module();
    auto gs = get_global_state_ro();

    ModuleContext mc{ gs.get(), module_bus };

    StatisticsMemory memory{ MemoryFactory::get_data_memory() };
    Storage storage{ &memory };
    if (!storage.begin()) {
        logerror("error opening storage, wiping...");
        if (!storage.clear()) {
            logerror("wiping storage failed!");
            return;
        }
    }

    auto data = storage.file(Storage::Data);
    auto meta = storage.file(Storage::Meta);

    ModuleScanning scanning{ get_modmux() };
    ModuleScan scan;
    if (!scanning.scan(scan)) {
        logerror("error scanning modules");
        return;
    }

    ModuleRegistry registry;
    ResolvedModules modules{ pool };
    if (!registry.resolve(scan, modules)) {
        logerror("error resolving modules");
        return;
    }

    append_configuration(mc, modules, meta, pool);

    Readings readings{ get_modmux() };
    if (!readings.take_readings(mc, modules, data.record(), pool)) {
        logerror("error taking readings");
        return;
    }

    append_readings(readings, data, pool);

    memory.log_statistics();

    loginfo("done (pool = %d/%d bytes) (%" PRIu32 "ms)", pool.used(), pool.size(), fk_uptime() - started);
}

static bool append_readings(Readings &readings, File &file, Pool &pool) {
    for (auto i = 0; i < FK_INFLATE_WRITES_READINGS; ++i) {
        auto bytes_wrote = file.write(&readings.record(), fk_data_DataRecord_fields);
        if (bytes_wrote == 0) {
            logerror("error saving readings");
            return false;
        }

        loginfo("wrote %d bytes (#%" PRIu32 ") (%" PRIu32 " bytes) (" PRADDRESS ")",
                bytes_wrote, file.record() - 1, file.size(), file.tail());
    }

    return true;
}

static bool append_configuration(ModuleContext &mc, ResolvedModules &modules, File &file, Pool &pool) {
    auto module_infos = pool.malloc<fk_data_ModuleInfo>(modules.size());

    auto index = 0;
    for (size_t i = 0; i < MaximumNumberOfModules; ++i) {
        auto meta = modules.meta(i);
        if (meta == nullptr) {
            continue;
        }

        auto module = modules.instance(i);
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

static void configure_module() {
    auto position = 6;
    ModuleHeader header = {
        .manufacturer = FK_MODULES_MANUFACTURER,
        .kind = FK_MODULES_KIND_WATER,
        .version = 0x01,
        .reserved = { 0x00, 0x00, 0x00, 0x00 },
        .crc = 0x00,
    };
    ModuleScanning scanning{ get_modmux() };
    if (!scanning.configure(position, header)) {
        logerror("[%d] unable to configure module", position);
        return;
    }
}

}
