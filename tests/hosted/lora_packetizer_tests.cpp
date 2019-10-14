#include <fk-data-protocol.h>

#include "tests.h"
#include "lora_packetizer.h"
#include "test_modules.h"
#include "modules/bridge/modules_bridge.h"

using namespace fk;

FK_DECLARE_LOGGER("tests");

class LoraPacketizerSuite : public ::testing::Test {
protected:

};

TEST_F(LoraPacketizerSuite, SingleReading) {
    StaticPool<1024> pool("Pool");

    auto module_readings = new (pool) NModuleReadings<4>();
    module_readings->set(0, 23.0f);

    ModuleReadingsCollection all_readings{ pool };
    all_readings.emplace_back(ModuleMetaAndReadings{
        .position = 0,
        .id = nullptr,
        .meta = &fk_test_module_fake_empty,
        .sensors = nullptr,
        .readings = module_readings,
    });

    LoraPacketizer packetizer;
    auto taken = TakenReadings{ 1571072401, 323432, all_readings };
    auto packets = packetizer.packetize(taken, pool);
    ASSERT_TRUE(packets);

    for (auto p = *packets; p != nullptr; p = p->link) {
        fk_dump_memory("packet ", p->buffer, p->size);
    }
}

TEST_F(LoraPacketizerSuite, OneModuleMultipleReadings) {
    StaticPool<1024> pool("Pool");

    auto module_readings = new (pool) NModuleReadings<5>();
    module_readings->set(0, 23.0f);
    module_readings->set(1, 332.0f);
    module_readings->set(2, 934839.0f);
    module_readings->set(3, 100.0f);
    module_readings->set(4, 39843.0f);

    ModuleReadingsCollection all_readings{ pool };
    all_readings.emplace_back(ModuleMetaAndReadings{
        .position = 0,
        .id = nullptr,
        .meta = &fk_test_module_fake_empty,
        .sensors = nullptr,
        .readings = module_readings,
    });

    LoraPacketizer packetizer;
    auto taken = TakenReadings{ 1571072401, 323432, all_readings };
    auto packets = packetizer.packetize(taken, pool);
    ASSERT_TRUE(packets);

    for (auto p = *packets; p != nullptr; p = p->link) {
        fk_dump_memory("packet ", p->buffer, p->size);
    }
}

TEST_F(LoraPacketizerSuite, TwoModulesMultipleReadings) {
    StaticPool<1024> pool("Pool");

    auto module_readings0 = new (pool) NModuleReadings<5>();
    module_readings0->set(0, 23.0f);
    module_readings0->set(1, 332.0f);
    module_readings0->set(2, 934839.0f);
    module_readings0->set(3, 100.0f);
    module_readings0->set(4, 39843.0f);

    auto module_readings1 = new (pool) NModuleReadings<3>();
    module_readings1->set(1, 23.0f);
    module_readings1->set(2, 100.0f);
    module_readings1->set(3, 39843.0f);

    ModuleReadingsCollection all_readings{ pool };
    all_readings.emplace_back(ModuleMetaAndReadings{
        .position = 0,
        .id = nullptr,
        .meta = &fk_test_module_fake_empty,
        .sensors = nullptr,
        .readings = module_readings0,
    });

    all_readings.emplace_back(ModuleMetaAndReadings{
        .position = 1,
        .id = nullptr,
        .meta = &fk_test_module_fake_empty,
        .sensors = nullptr,
        .readings = module_readings1,
    });

    LoraPacketizer packetizer;
    auto taken = TakenReadings{ 1571072401, 323432, all_readings };
    auto packets = packetizer.packetize(taken, pool);
    ASSERT_TRUE(packets);

    for (auto p = *packets; p != nullptr; p = p->link) {
        fk_dump_memory("packet ", p->buffer, p->size);
    }
}

TEST_F(LoraPacketizerSuite, MultipleModulesVerifyLength) {
    StaticPool<1024> pool("Pool");

    // auto module_readings0 = new (pool) NModuleReadings<0>();
    // auto module_readings1 = new (pool) NModuleReadings<0>();
    auto module_readings2 = new (pool) NModuleReadings<3>();
    module_readings2->set(0, 1725.000000);
    module_readings2->set(1, 1726.000000);
    module_readings2->set(2, 1727.000000);

    auto i = 0u;
    auto module_readings3 = new (pool) NModuleReadings<13>();
    module_readings3->set(i++, 24.196228);
    module_readings3->set(i++, 100.226753);
    module_readings3->set(i++, 22.937500);
    module_readings3->set(i++, 7.200000);
    module_readings3->set(i++, 0.000000);
    module_readings3->set(i++, 157.000000);
    module_readings3->set(i++, 309.375000);
    module_readings3->set(i++, 9.600000);
    module_readings3->set(i++, 9.600000);
    module_readings3->set(i++, 8.067230);
    module_readings3->set(i++, 147.000000);

    ModuleReadingsCollection all_readings{ pool };
    all_readings.emplace_back(ModuleMetaAndReadings{
        .position = 2,
        .id = nullptr,
        .meta = &fk_test_module_fake_empty,
        .sensors = nullptr,
        .readings = module_readings2,
    });

    all_readings.emplace_back(ModuleMetaAndReadings{
        .position = 6,
        .id = nullptr,
        .meta = &fk_test_module_fake_empty,
        .sensors = nullptr,
        .readings = module_readings3,
    });

    LoraPacketizer packetizer;
    auto taken = TakenReadings{ 1571073001, 7187, all_readings };
    auto packets = packetizer.packetize(taken, pool);
    ASSERT_TRUE(packets);

    for (auto p = *packets; p != nullptr; p = p->link) {
        fk_dump_memory("packet ", p->buffer, p->size);
    }
}
