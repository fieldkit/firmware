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

TEST_F(LoraPacketizerSuite, Basic) {
    StaticPool<1024> pool("Pool");

    auto module_readings = new (pool) NModuleReadings<4>();
    module_readings->set(0, 23.0f);
    module_readings->set(1, 332.0f);
    module_readings->set(2, 934839.0f);
    module_readings->set(3, 100.0f);

    ModuleReadingsCollection all_readings{ pool };
    all_readings.emplace_back(ModuleMetaAndReadings{
        .position = 0,
        .id = nullptr,
        .meta = &fk_test_module_fake_empty,
        .sensors = nullptr,
        .readings = module_readings,
    });

    LoraPacketizer packetizer;
    packetizer.packetize(all_readings, pool);
}
