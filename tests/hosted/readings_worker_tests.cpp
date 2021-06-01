#include "tests.h"
#include "patterns.h"
#include "common.h"
#include "hal/linux/linux.h"
#include "startup_worker.h"
#include "readings_worker.h"
#include "state_manager.h"
#include "state_ref.h"

#include "storage_suite.h"

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

TEST_F(ReadingsWorkerSuite, TakingFirstReading) {
    StandardPool pool{ "tests" };
    StartupWorker startup_worker{ true };
    startup_worker.run(pool);

    auto gs = get_global_state_ro();

    ASSERT_EQ(gs.get()->readings.nreadings, 0u);

    ReadingsWorker readings_worker{ true, false, ModulePowerState::Unknown };
    readings_worker.run(pool);

    ASSERT_EQ(gs.get()->readings.nreadings, 1u);
}

TEST_F(ReadingsWorkerSuite, TakingSecondReading) {
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
