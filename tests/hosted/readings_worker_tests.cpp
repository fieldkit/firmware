#include "tests.h"
#include "patterns.h"
#include "common.h"
#include "hal/linux/linux.h"
#include "startup_worker.h"
#include "readings_worker.h"
#include "state_manager.h"
#include "state_ref.h"
#include "storage_suite.h"
#include "test_modules.h"

using namespace fk;

static fkb_header_t fake_header = {
    .signature          = { 'F', 'K', 'B', 0 },
    .version            = 1,
    .size               = sizeof(fkb_header_t),
    .firmware           = {
        .flags          = 0,
        .timestamp      = 1580763366,
        .number         = 1000,
        .reserved       = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff },
        .safe           = 0xff,
        .previous       = UINT32_MAX,
        .binary_size    = 65536,
        .tables_offset  = 8192,
        .data_size      = 8192,
        .bss_size       = 8192,
        .got_size       = 8192,
        .vtor_offset    = 8192,
        .got_offset     = 32768,
        .version        = { 0x0 },
        .hash_size      = 32,
        .hash           = { 0xB2 }
    },
    .number_symbols     = 100,
    .number_relocations = 100
};

FK_DECLARE_LOGGER("readings-worker-tests");

class ReadingsWorkerSuite : public StorageSuite {
};

TEST_F(ReadingsWorkerSuite, OnlyDiagnosticsModule_FirstReading) {
    StandardPool pool{ "tests" };
    StartupWorker startup_worker{ true };
    startup_worker.fkb_header(&fake_header);
    startup_worker.run(pool);

    auto gs = get_global_state_ro();

    ASSERT_EQ(gs.get()->readings.nreadings, 0u);

    ReadingsWorker readings_worker{ true, false, ModulePowerState::Unknown };
    readings_worker.run(pool);

    ASSERT_EQ(gs.get()->readings.nreadings, 1u);
}

TEST_F(ReadingsWorkerSuite, OnlyDiagnosticsModule_SecondReading) {
    auto gs = get_global_state_ro();

    StandardPool pool{ "tests" };
    StartupWorker startup_worker{ true };
    startup_worker.fkb_header(&fake_header);
    startup_worker.run(pool);

    ASSERT_EQ(gs.get()->readings.nreadings, 0u);

    ReadingsWorker readings_worker{ true, false, ModulePowerState::Unknown };
    readings_worker.run(pool);

    ASSERT_EQ(gs.get()->readings.nreadings, 1u);

    readings_worker.run(pool);

    ASSERT_EQ(gs.get()->readings.nreadings, 2u);
}

TEST_F(ReadingsWorkerSuite, ScannedModule_InvalidHeader) {
    auto mm = (LinuxModMux *)get_modmux();

    ModuleHeader header;
    bzero(&header, sizeof(ModuleHeader));
    mm->set_eeprom_data(ModulePosition::from(2), (uint8_t *)&header, sizeof(header));

    auto gs = get_global_state_ro();

    StandardPool pool{ "tests" };
    StartupWorker startup_worker{ true };
    startup_worker.fkb_header(&fake_header);
    startup_worker.run(pool);

    ASSERT_EQ(gs.get()->readings.nreadings, 0u);

    fk_fake_uptime({ 20321 });

    ReadingsWorker readings_worker{ true, false, ModulePowerState::Unknown };
    readings_worker.run(pool);

    ASSERT_EQ(gs.get()->readings.nreadings, 1u);
}

TEST_F(ReadingsWorkerSuite, ScannedModule_MultipleReadings) {
    auto mm = (LinuxModMux *)get_modmux();

    ModuleHeader header;
    bzero(&header, sizeof(ModuleHeader));
    header.manufacturer = FK_MODULES_MANUFACTURER;
    header.kind = FK_MODULES_KIND_RANDOM;
    header.version = 0x02;
    header.crc = fk_module_header_sign(&header);
    mm->set_eeprom_data(ModulePosition::from(2), (uint8_t *)&header, sizeof(header));

    fk_modules_builtin_register(&fk_test_module_fake_1);

    auto gs = get_global_state_ro();

    fk_fake_uptime({ 20321 });

    StandardPool pool{ "tests" };
    StartupWorker startup_worker{ true };
    startup_worker.fkb_header(&fake_header);
    startup_worker.run(pool);

    ASSERT_EQ(gs.get()->readings.nreadings, 0u);

    ReadingsWorker readings_worker{ true, false, ModulePowerState::Unknown };
    readings_worker.run(pool);

    ASSERT_EQ(gs.get()->readings.nreadings, 1u);

    fk_fake_uptime({ 30321 });

    readings_worker.run(pool);

    ASSERT_EQ(gs.get()->readings.nreadings, 2u);

    fk_fake_uptime({ 40321 });

    readings_worker.run(pool);

    ASSERT_EQ(gs.get()->readings.nreadings, 3u);

    {
        StandardPool pool{ "tests" };
        Storage storage{ memory_, pool, false };

        ASSERT_TRUE(storage.begin());

        auto reader = storage.file_reader(Storage::Data, pool);
        ASSERT_NE(reader, nullptr);

        ASSERT_TRUE(reader->seek_record(0, pool));

        MetaRecord meta_record;

        // State record
        auto bytes_read = reader->read(&meta_record.for_decoding(pool), fk_data_DataRecord_fields);
        ASSERT_EQ(bytes_read, 320);

        // Modules record
        bytes_read = reader->read(&meta_record.for_decoding(pool), fk_data_DataRecord_fields);
        ASSERT_EQ(bytes_read, 434);

        // Data record
        bytes_read = reader->read(&meta_record.for_decoding(pool), fk_data_DataRecord_fields);
        ASSERT_EQ(bytes_read, 91);
        ASSERT_EQ(meta_record.record().readings.reading, 2u);
        ASSERT_EQ(meta_record.record().readings.meta, 1u);
        ASSERT_EQ(meta_record.record().readings.uptime, 20321u);

        // Data record
        bytes_read = reader->read(&meta_record.for_decoding(pool), fk_data_DataRecord_fields);
        ASSERT_EQ(bytes_read, 91);
        ASSERT_EQ(meta_record.record().readings.reading, 3u);
        ASSERT_EQ(meta_record.record().readings.meta, 1u);
        ASSERT_EQ(meta_record.record().readings.uptime, 30321u);

        // Data record
        bytes_read = reader->read(&meta_record.for_decoding(pool), fk_data_DataRecord_fields);
        ASSERT_EQ(bytes_read, 91);
        ASSERT_EQ(meta_record.record().readings.reading, 4u);
        ASSERT_EQ(meta_record.record().readings.meta, 1u);
        ASSERT_EQ(meta_record.record().readings.uptime, 40321u);

        // End of file
        bytes_read = reader->read(&meta_record.for_decoding(pool), fk_data_DataRecord_fields);
        ASSERT_EQ(bytes_read, -1);
    }
}

TEST_F(ReadingsWorkerSuite, ScannedModule_ModuleAdded) {
    auto mm = (LinuxModMux *)get_modmux();

    ModuleHeader header;
    bzero(&header, sizeof(ModuleHeader));
    header.manufacturer = FK_MODULES_MANUFACTURER;
    header.kind = FK_MODULES_KIND_RANDOM;
    header.version = 0x02;
    header.crc = fk_module_header_sign(&header);

    fk_modules_builtin_register(&fk_test_module_fake_1);

    auto gs = get_global_state_ro();

    StandardPool pool{ "tests" };
    StartupWorker startup_worker{ true };
    startup_worker.fkb_header(&fake_header);
    startup_worker.run(pool);

    ASSERT_EQ(gs.get()->readings.nreadings, 0u);

    ReadingsWorker readings_worker{ true, false, ModulePowerState::Unknown };

    fk_fake_uptime({ 20321 });

    readings_worker.run(pool);

    ASSERT_EQ(gs.get()->readings.nreadings, 1u);

    // Add the module
    mm->set_eeprom_data(ModulePosition::from(2), (uint8_t *)&header, sizeof(header));

    readings_worker.run(pool);

    ASSERT_EQ(gs.get()->readings.nreadings, 2u);

    {
        StandardPool pool{ "tests" };
        Storage storage{ memory_, pool, false };

        ASSERT_TRUE(storage.begin());

        auto reader = storage.file_reader(Storage::Data, pool);
        ASSERT_NE(reader, nullptr);

        ASSERT_TRUE(reader->seek_record(0, pool));

        MetaRecord meta_record;

        // State record
        auto bytes_read = reader->read(&meta_record.for_decoding(pool), fk_data_DataRecord_fields);
        ASSERT_EQ(bytes_read, 320);

        // Modules record
        bytes_read = reader->read(&meta_record.for_decoding(pool), fk_data_DataRecord_fields);
        ASSERT_EQ(bytes_read, 375);

        // Data record
        bytes_read = reader->read(&meta_record.for_decoding(pool), fk_data_DataRecord_fields);
        ASSERT_EQ(bytes_read, 80);
        ASSERT_EQ(meta_record.record().readings.reading, 2u);
        ASSERT_EQ(meta_record.record().readings.meta, 1u);
        ASSERT_EQ(meta_record.record().readings.uptime, 20321u);

        // Modules record
        bytes_read = reader->read(&meta_record.for_decoding(pool), fk_data_DataRecord_fields);
        ASSERT_EQ(bytes_read, 434);

        // Data record
        bytes_read = reader->read(&meta_record.for_decoding(pool), fk_data_DataRecord_fields);
        ASSERT_EQ(bytes_read, 91);
        ASSERT_EQ(meta_record.record().readings.reading, 4u);
        ASSERT_EQ(meta_record.record().readings.meta, 3u);
        ASSERT_EQ(meta_record.record().readings.uptime, 20321u);

        // End of file
        bytes_read = reader->read(&meta_record.for_decoding(pool), fk_data_DataRecord_fields);
        ASSERT_EQ(bytes_read, -1);
    }
}
