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

    ModuleHeader header;
    state::DynamicState dynamic;
    state::AttachedModule am{ ModulePosition::from(0), header, nullptr, nullptr, pool };
    am.add_sensor(state::AttachedSensor{ nullptr, 0, ModuleReading{ 23.0f } });
    dynamic.attached()->add_module(am);

    GlobalState gs;
    gs.readings.time = 1600000000;
    gs.readings.nreadings = 100;
    gs.dynamic = std::move(dynamic);

    LoraPacketizer packetizer;
    auto packets = packetizer.packetize(&gs, pool);
    ASSERT_TRUE(packets);

    for (auto p = *packets; p != nullptr; p = p->link) {
        fk_dump_memory("packet ", p->buffer, p->size);
    }
}

TEST_F(LoraPacketizerSuite, OneModuleMultipleReadings) {
    StaticPool<1024> pool("Pool");

    ModuleHeader header;
    state::DynamicState dynamic;
    state::AttachedModule am{ ModulePosition::from(0), header, nullptr, nullptr, pool };
    am.add_sensor(state::AttachedSensor{ nullptr, 0, ModuleReading{ 23.0f } });
    am.add_sensor(state::AttachedSensor{ nullptr, 1, ModuleReading{ 332.0f } });
    am.add_sensor(state::AttachedSensor{ nullptr, 2, ModuleReading{ 9348839.0f } });
    am.add_sensor(state::AttachedSensor{ nullptr, 3, ModuleReading{ 100.0f } });
    am.add_sensor(state::AttachedSensor{ nullptr, 4, ModuleReading{ 39843.0f } });
    dynamic.attached()->add_module(am);

    GlobalState gs;
    gs.readings.time = 1600000000;
    gs.readings.nreadings = 100;
    gs.dynamic = std::move(dynamic);

    LoraPacketizer packetizer;
    auto packets = packetizer.packetize(&gs, pool);
    ASSERT_TRUE(packets);

    for (auto p = *packets; p != nullptr; p = p->link) {
        fk_dump_memory("packet ", p->buffer, p->size);
    }
}

TEST_F(LoraPacketizerSuite, TwoModulesMultipleReadings) {
    StaticPool<1024> pool("Pool");

    ModuleHeader header;
    state::DynamicState dynamic;
    state::AttachedModule am1{ ModulePosition::from(0), header, nullptr, nullptr, pool };
    am1.add_sensor(state::AttachedSensor{ nullptr, 0, ModuleReading{ 23.0f } });
    am1.add_sensor(state::AttachedSensor{ nullptr, 1, ModuleReading{ 332.0f } });
    am1.add_sensor(state::AttachedSensor{ nullptr, 2, ModuleReading{ 9348839.0f } });
    am1.add_sensor(state::AttachedSensor{ nullptr, 3, ModuleReading{ 100.0f } });
    am1.add_sensor(state::AttachedSensor{ nullptr, 4, ModuleReading{ 39843.0f } });
    dynamic.attached()->add_module(am1);
    state::AttachedModule am2{ ModulePosition::from(1), header, nullptr, nullptr, pool };
    am2.add_sensor(state::AttachedSensor{ nullptr, 0, ModuleReading{ 23.0f } });
    am2.add_sensor(state::AttachedSensor{ nullptr, 1, ModuleReading{ 100.0f } });
    am2.add_sensor(state::AttachedSensor{ nullptr, 2, ModuleReading{ 39843.0f } });
    dynamic.attached()->add_module(am2);

    GlobalState gs;
    gs.readings.time = 1600000000;
    gs.readings.nreadings = 100;
    gs.dynamic = std::move(dynamic);

    LoraPacketizer packetizer;
    auto packets = packetizer.packetize(&gs, pool);
    ASSERT_TRUE(packets);

    for (auto p = *packets; p != nullptr; p = p->link) {
        fk_dump_memory("packet ", p->buffer, p->size);
    }
}

TEST_F(LoraPacketizerSuite, MultipleModulesVerifyLength1) {
    StaticPool<1024> pool("Pool");

    ModuleHeader header;
    state::DynamicState dynamic;
    state::AttachedModule am1{ ModulePosition::from(0), header, nullptr, nullptr, pool };
    am1.add_sensor(state::AttachedSensor{ nullptr, 0, ModuleReading{ 1725.0f } });
    am1.add_sensor(state::AttachedSensor{ nullptr, 1, ModuleReading{ 1726.0f } });
    am1.add_sensor(state::AttachedSensor{ nullptr, 2, ModuleReading{ 1727.0f } });
    dynamic.attached()->add_module(am1);
    auto i = 0u;
    state::AttachedModule am2{ ModulePosition::from(1), header, nullptr, nullptr, pool };
    am2.add_sensor(state::AttachedSensor{ nullptr, i++, ModuleReading{ 24.196228 } });
    am2.add_sensor(state::AttachedSensor{ nullptr, i++, ModuleReading{ 100.226753 } });
    am2.add_sensor(state::AttachedSensor{ nullptr, i++, ModuleReading{ 22.937500 } });
    am2.add_sensor(state::AttachedSensor{ nullptr, i++, ModuleReading{ 7.200000 } });
    am2.add_sensor(state::AttachedSensor{ nullptr, i++, ModuleReading{ 0.000000 } });
    am2.add_sensor(state::AttachedSensor{ nullptr, i++, ModuleReading{ 157.000000 } });
    am2.add_sensor(state::AttachedSensor{ nullptr, i++, ModuleReading{ 309.375000 } });
    am2.add_sensor(state::AttachedSensor{ nullptr, i++, ModuleReading{ 9.600000 } });
    am2.add_sensor(state::AttachedSensor{ nullptr, i++, ModuleReading{ 9.600000 } });
    am2.add_sensor(state::AttachedSensor{ nullptr, i++, ModuleReading{ 8.067230 } });
    am2.add_sensor(state::AttachedSensor{ nullptr, i++, ModuleReading{ 147.000000 } });
    dynamic.attached()->add_module(am2);

    GlobalState gs;
    gs.readings.time = 1600000000;
    gs.readings.nreadings = 100;
    gs.dynamic = std::move(dynamic);

    LoraPacketizer packetizer;
    auto packets = packetizer.packetize(&gs, pool);
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

    ModuleHeader header;
    state::DynamicState dynamic;
    state::AttachedModule am1{ ModulePosition::from(2), header, nullptr, nullptr, pool };
    am1.add_sensor(state::AttachedSensor{ nullptr, 0, ModuleReading{ 1726.0f } });
    am1.add_sensor(state::AttachedSensor{ nullptr, 1, ModuleReading{ 1727.0f } });
    am1.add_sensor(state::AttachedSensor{ nullptr, 2, ModuleReading{ 1728.0f } });
    dynamic.attached()->add_module(am1);

    GlobalState gs;
    gs.readings.time = 1600000000;
    gs.readings.nreadings = 100;
    gs.dynamic = std::move(dynamic);

    LoraPacketizer packetizer;
    auto packets = packetizer.packetize(&gs, pool);
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

TEST_F(LoraPacketizerSuite, MultipleModulesVerifyLength3) {
    StaticPool<1024> pool("Pool");

    ModuleHeader header;
    state::DynamicState dynamic;
    state::AttachedModule am1{ ModulePosition::from(2), header, nullptr, nullptr, pool };
    am1.add_sensor(state::AttachedSensor{ nullptr, 0, ModuleReading{ 1221.0f } });
    am1.add_sensor(state::AttachedSensor{ nullptr, 1, ModuleReading{ 1222.0f } });
    am1.add_sensor(state::AttachedSensor{ nullptr, 2, ModuleReading{ 1222.0f } });
    dynamic.attached()->add_module(am1);
    state::AttachedModule am2{ ModulePosition::from(6), header, nullptr, nullptr, pool };
    auto i = 0u;
    am2.add_sensor(state::AttachedSensor{ nullptr, i++, ModuleReading{ 44.954605 } });
    am2.add_sensor(state::AttachedSensor{ nullptr, i++, ModuleReading{ 24.110779 } });
    am2.add_sensor(state::AttachedSensor{ nullptr, i++, ModuleReading{ 100.027252 } });
    am2.add_sensor(state::AttachedSensor{ nullptr, i++, ModuleReading{ 22.812500 } });
    am2.add_sensor(state::AttachedSensor{ nullptr, i++, ModuleReading{ 0.0 } });
    am2.add_sensor(state::AttachedSensor{ nullptr, i++, ModuleReading{ 9.600000 } });
    am2.add_sensor(state::AttachedSensor{ nullptr, i++, ModuleReading{ 0.0 } });
    am2.add_sensor(state::AttachedSensor{ nullptr, i++, ModuleReading{ 2307.421875 } });
    am2.add_sensor(state::AttachedSensor{ nullptr, i++, ModuleReading{ 9.600000 } });
    am2.add_sensor(state::AttachedSensor{ nullptr, i++, ModuleReading{ 0.000000 } });
    am2.add_sensor(state::AttachedSensor{ nullptr, i++, ModuleReading{ 0.000000 } });
    dynamic.attached()->add_module(am2);

    GlobalState gs;
    gs.readings.time = 1600000000;
    gs.readings.nreadings = 100;
    gs.dynamic = std::move(dynamic);

    LoraPacketizer packetizer;
    auto packets = packetizer.packetize(&gs, pool);
    ASSERT_TRUE(packets);

    for (auto p = *packets; p != nullptr; p = p->link) {
        fk_dump_memory("packet ", p->buffer, p->size);
    }
}

TEST_F(LoraPacketizerSuite, QuickDecode) {
    StaticPool<1024> pool("Pool");

    const char *messages[] = {
        "0005d414f0a78640",
    };

    uint8_t buffer[32];
    hex_string_to_bytes(buffer, sizeof(buffer), messages[0]);

    int32_t error = 0;
    auto age = phylum::varint_decode(buffer + 1, sizeof(buffer) - 1, &error);
    auto age_length = phylum::varint_encoding_length(age);

    ASSERT_EQ(age, 5u);

    auto reading = phylum::varint_decode(buffer + 1 + age_length, sizeof(buffer) - 1 - age_length, &error);
    auto reading_length = phylum::varint_encoding_length(reading);

    ASSERT_EQ(reading, 2644u);
    ASSERT_EQ(reading_length, 2u);
}