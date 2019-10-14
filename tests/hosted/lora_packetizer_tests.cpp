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

TEST_F(LoraPacketizerSuite, MultipleModulesVerifyLength1) {
    StaticPool<1024> pool("Pool");

    auto module_readings0 = new (pool) NModuleReadings<3>();
    module_readings0->set(0, 1725.000000);
    module_readings0->set(1, 1726.000000);
    module_readings0->set(2, 1727.000000);

    auto i = 0u;
    auto module_readings1 = new (pool) NModuleReadings<13>();
    module_readings1->set(i++, 24.196228);
    module_readings1->set(i++, 100.226753);
    module_readings1->set(i++, 22.937500);
    module_readings1->set(i++, 7.200000);
    module_readings1->set(i++, 0.000000);
    module_readings1->set(i++, 157.000000);
    module_readings1->set(i++, 309.375000);
    module_readings1->set(i++, 9.600000);
    module_readings1->set(i++, 9.600000);
    module_readings1->set(i++, 8.067230);
    module_readings1->set(i++, 147.000000);

    ModuleReadingsCollection all_readings{ pool };
    all_readings.emplace_back(ModuleMetaAndReadings{
        .position = 2,
        .id = nullptr,
        .meta = &fk_test_module_fake_empty,
        .sensors = nullptr,
        .readings = module_readings0,
    });

    all_readings.emplace_back(ModuleMetaAndReadings{
        .position = 6,
        .id = nullptr,
        .meta = &fk_test_module_fake_empty,
        .sensors = nullptr,
        .readings = module_readings1,
    });

    LoraPacketizer packetizer;
    auto taken = TakenReadings{ 1571073001, 7187, all_readings };
    auto packets = packetizer.packetize(taken, pool);
    ASSERT_TRUE(packets);

    for (auto p = *packets; p != nullptr; p = p->link) {
        fk_dump_memory("packet ", p->buffer, p->size);
    }
}

TEST_F(LoraPacketizerSuite, MultipleModulesVerifyLength2) {
    StaticPool<1024> pool("Pool");

    // 00027444 lora       debug   packetizer: begin time=7435 reading=0
    // 00027444 lora       debug   packetizer: reading: 2/0 1726.000000
    // 00027444 lora       debug   packetizer: reading: 2/1 1727.000000
    // 00027444 lora       debug   packetizer: reading: 2/2 1728.000000
    // PTR 00 c0 d7 44 00 e0 d7 44 00 00 d8 44  (12 bytes)
    // PTR 00 c0 d7 44 00 e0 d7 44 00 00 d8 44  (12 bytes)
    // 00027445 lora       warn    packetizer: encoded size differs from predicted (19 != 21)
    // LORA 10 8b 3a 20 02 32 0c 00 c0 d7 44 00 e0 d7 44 00 00 d8 44  (19 bytes)

    auto module_readings0 = new (pool) NModuleReadings<3>();
    module_readings0->set(0, 1726.000000);
    module_readings0->set(1, 1727.000000);
    module_readings0->set(2, 1728.000000);

    ModuleReadingsCollection all_readings{ pool };
    all_readings.emplace_back(ModuleMetaAndReadings{
        .position = 2,
        .id = nullptr,
        .meta = &fk_test_module_fake_empty,
        .sensors = nullptr,
        .readings = module_readings0,
    });

    LoraPacketizer packetizer;
    auto taken = TakenReadings{ 7435, 0, all_readings };
    auto packets = packetizer.packetize(taken, pool);
    ASSERT_TRUE(packets);

    for (auto p = *packets; p != nullptr; p = p->link) {
        fk_dump_memory("packet ", p->buffer, p->size);
    }
}

/*
00055214 lora       debug   packetizer: begin time=25531 reading=7350
00055214 lora       debug   packetizer: reading: 2/0 1726.000000
00055214 lora       debug   packetizer: reading: 2/1 1726.000000
00055214 lora       debug   packetizer: reading: 2/2 1731.000000
PTR 00 c0 d7 44 00 c0 d7 44 00 60 d8 44  (12 bytes)
PTR 00 c0 d7 44 00 c0 d7 44 00 60 d8 44  (12 bytes)
00055216 lora       debug   packetizer: reading: 6/0 45.546654
00055216 lora       debug   packetizer: reading: 6/1 23.531322
00055216 lora       debug   packetizer: reading: 6/2 100.158249
00055216 lora       debug   packetizer: reading: 6/3 22.250000
PTR c6 2f 36 42 26 40 bc 41 06 51 c8 42 00 00 b2 41  (16 bytes)
PTR c6 2f 36 42 26 40 bc 41 06 51 c8 42 00 00 b2 41  (16 bytes)
00055217 lora       warn    packetizer: encoded size differs from predicted (20 != 22)
00055217 lora       debug   packetizer: reading: 6/5 9.600000
00055217 lora       debug   packetizer: reading: 6/6 337.000000
00055217 lora       debug   packetizer: reading: 6/7 1830.468750
PTR 9a 99 19 41 00 80 a8 43 00 cf e4 44  (12 bytes)
PTR 9a 99 19 41 00 80 a8 43 00 cf e4 44  (12 bytes)
00055218 lora       debug   packetizer: reading: 6/8 9.600000
00055218 lora       debug   packetizer: reading: 6/9 202.000000
00055218 lora       debug   packetizer: reading: 6/10 9.600000
PTR 9a 99 19 41 00 00 4a 43 9a 99 19 41  (12 bytes)
PTR 9a 99 19 41 00 00 4a 43 9a 99 19 41  (12 bytes)
00055219 lora       debug   packetizer: reading: 6/11 202.000000
00055219 lora       debug   packetizer: reading: 6/12 7.600000
00055219 lora       debug   packetizer: reading: 6/13 312.000000
PTR 00 00 4a 43 33 33 f3 40 00 00 9c 43  (12 bytes)
PTR 00 00 4a 43 33 33 f3 40 00 00 9c 43  (12 bytes)
LORA 10 bb c7 01 18 b6 39 20 02 32 0c 00 c0 d7 44 00 c0 d7 44 00 60 d8 44  (23 bytes)
LORA 20 06 32 10 c6 2f 36 42 26 40 bc 41 06 51 c8 42 00 00 b2 41  (20 bytes)
LORA 20 06 28 05 32 0c 9a 99 19 41 00 80 a8 43 00 cf e4 44  (18 bytes)
LORA 20 06 28 08 32 0c 9a 99 19 41 00 00 4a 43 9a 99 19 41  (18 bytes)
LORA 20 06 28 0b 32 0c 00 00 4a 43 33 33 f3 40 00 00 9c 43  (18 bytes)
*/
