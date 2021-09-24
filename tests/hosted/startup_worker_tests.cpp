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

FK_DECLARE_LOGGER("startup-worker-tests");

class StartupWorkerSuite : public StorageSuite {
};

TEST_F(StartupWorkerSuite, FreshStart) {
    StandardPool pool{ "tests" };
    StartupWorker startup_worker{ true };
    startup_worker.run(pool);
}

TEST_F(StartupWorkerSuite, SecondStart_KeepsName) {
    char name[128];

    {
        StandardPool pool{ "tests" };
        StartupWorker startup_worker{ true };
        startup_worker.run(pool);

        auto gs = get_global_state_ro();
        strncpy(name, gs.get()->general.name, sizeof(name));
    }

    {
        StandardPool pool{ "tests" };
        StartupWorker startup_worker{ true };
        startup_worker.run(pool);

        auto gs = get_global_state_ro();
        ASSERT_STREQ(name, gs.get()->general.name);
    }
}
