#include "tests.h"
#include "patterns.h"

#include "data_writer.h"

#include "hal/linux/linux.h"
#include "storage_suite.h"
#include "export_data_worker.h"

extern "C" __attribute__((constructor)) void fk_module_initialize_random();

using namespace fk;

FK_DECLARE_LOGGER("tests");

class ExportDataSuite : public StorageSuite {
protected:
    void SetUp() override {
        StorageSuite::SetUp();
        fk_module_initialize_random();
    }

    void TearDown() override {
        fk_modules_builtin_clear();
        StorageSuite::TearDown();
    }

};

TEST_F(ExportDataSuite, ExportingEmpty) {
    DataWriter writer{ memory_ };
    for (auto i = 0u; i < 100u; ++i) {
        StandardPool loop_pool{ "loop" };
        ASSERT_TRUE(writer.write(loop_pool));
    }

    enable_info();

    ExportDataWorker worker{ memory_ };
    worker.run(pool_);
}
