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

FK_DECLARE_LOGGER("readings-worker-tests");

class ReadingsWorkerSuite : public StorageSuite {
protected:
    void SetUp() override {
        StorageSuite::SetUp();
    }

    void TearDown() override {
        StorageSuite::TearDown();
    }
};

TEST_F(ReadingsWorkerSuite, OnlyDiagnosticsModule_FirstReading) {
    StandardPool pool{ "tests" };
    StartupWorker startup_worker{ true };
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
    startup_worker.run(pool);

    ASSERT_EQ(gs.get()->readings.nreadings, 0u);

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

    StandardPool pool{ "tests" };
    StartupWorker startup_worker{ true };
    startup_worker.run(pool);

    ASSERT_EQ(gs.get()->readings.nreadings, 0u);

    ReadingsWorker readings_worker{ true, false, ModulePowerState::Unknown };
    readings_worker.run(pool);

    ASSERT_EQ(gs.get()->readings.nreadings, 1u);

    readings_worker.run(pool);

    ASSERT_EQ(gs.get()->readings.nreadings, 2u);

    readings_worker.run(pool);

    ASSERT_EQ(gs.get()->readings.nreadings, 3u);
}
