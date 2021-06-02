#include "tests.h"
#include "patterns.h"

#include "data_writer.h"

#include "common.h"
#include "hal/linux/linux.h"
#include "storage_suite.h"
#include "export_data_worker.h"

using namespace fk;

FK_DECLARE_LOGGER("tests");

class ExportDataSuite : public StorageSuite {
protected:
    void SetUp() override {
        StorageSuite::SetUp();
        Storage storage{ memory_, pool_ };
        ASSERT_TRUE(storage.clear());
    }

    void TearDown() override {
        StorageSuite::TearDown();
    }

};

TEST_F(ExportDataSuite, ExportingEmpty) {
    ExportDataWorker worker{ memory_ };
    worker.run(pool_);
}

TEST_F(ExportDataSuite, ExportingOneRecord) {
    DataWriter writer{ memory_ };
    StandardPool loop_pool{ "loop" };
    ASSERT_TRUE(writer.write(loop_pool, 1));

    ExportDataWorker worker{ memory_ };
    worker.run(pool_);
}

TEST_F(ExportDataSuite, Exporting100OfSameModuleLayout) {
    DataWriter writer{ memory_ };
    for (auto i = 0u; i < 100u; ++i) {
        StandardPool loop_pool{ "loop" };
        ASSERT_TRUE(writer.write(loop_pool, 1));
    }

    ExportDataWorker worker{ memory_ };
    worker.run(pool_);
}

TEST_F(ExportDataSuite, Exporting100OfTwoModuleLayouts) {
    DataWriter writer{ memory_ };
    for (auto i = 0u; i < 50u; ++i) {
        StandardPool loop_pool{ "loop" };
        ASSERT_TRUE(writer.write(loop_pool, 1));
    }
    for (auto i = 0u; i < 50u; ++i) {
        StandardPool loop_pool{ "loop" };
        ASSERT_TRUE(writer.write(loop_pool, 2));
    }

    ExportDataWorker worker{ memory_ };
    worker.run(pool_);
}

TEST_F(ExportDataSuite, Exporting100OfThreeModuleLayouts) {
    DataWriter writer{ memory_ };
    for (auto i = 0u; i < 50u; ++i) {
        StandardPool loop_pool{ "loop" };
        ASSERT_TRUE(writer.write(loop_pool, 1));
    }
    for (auto i = 0u; i < 50u; ++i) {
        StandardPool loop_pool{ "loop" };
        ASSERT_TRUE(writer.write(loop_pool, 2));
    }
    for (auto i = 0u; i < 20u; ++i) {
        StandardPool loop_pool{ "loop" };
        ASSERT_TRUE(writer.write(loop_pool, 1));
    }

    ExportDataWorker worker{ memory_ };
    worker.run(pool_);
}
