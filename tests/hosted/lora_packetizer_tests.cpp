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
    auto packets = packetizer.packetize(all_readings, pool);
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
    auto packets = packetizer.packetize(all_readings, pool);
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
    auto packets = packetizer.packetize(all_readings, pool);
    ASSERT_TRUE(packets);

    for (auto p = *packets; p != nullptr; p = p->link) {
        fk_dump_memory("packet ", p->buffer, p->size);
    }
}
