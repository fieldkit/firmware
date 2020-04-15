#include <fstream>

#include <fk-data-protocol.h>

#include "readings_taker.h"
#include "networking/http_reply.h"
#include "storage/signed_log.h"
#include "storage/meta_record.h"

#include "storage_suite.h"

#include "test_modules.h"

using namespace fk;

fkb_header_t fake_header = {
    .signature          = { 'F', 'K', 'B', 0 },
    .version            = 1,
    .size               = sizeof(fkb_header_t),
    .firmware           = {
        .flags          = 0,
        .timestamp      = 1580763366,
        .number         = 1000,
        .version        = { 0x0 },
        .binary_size    = 65536,
        .tables_offset  = 8192,
        .data_size      = 8192,
        .bss_size       = 8192,
        .got_size       = 8192,
        .vtor_offset    = 8192,
        .got_offset     = 32768,
        .name           = { 0x0 },
        .hash_size      = 32,
        .hash           = { 0xB2 }
    },
    .number_symbols     = 100,
    .number_relocations = 100
};

FK_DECLARE_LOGGER("protobuf");

template<typename T = uint8_t, size_t Size>
static void fake_data(T(&buffer)[Size]) {
    for (auto i = 0u; i < Size; ++i) {
        buffer[i] = i % 255;
    }
}

template<typename T = char, size_t Size>
static void fake_string(T(&buffer)[Size]) {
    for (auto i = 0u; i < Size - 1; ++i) {
        buffer[i] = 'a' + (i % 10);
    }
    buffer[Size - 1] = 0;
}

static void fake_global_state(GlobalState &gs, Pool &pool) {
    gs.version = 100;
    fake_string(gs.general.name);
    fake_data(gs.general.generation);

    gs.general.recording = 1580763366;

    gs.lora.configured = true;
    gs.lora.uplink_counter = 4096;
    gs.lora.downlink_counter = 1024;
    fake_data(gs.lora.device_eui);
    fake_data(gs.lora.app_eui);
    fake_data(gs.lora.app_key);
    fake_data(gs.lora.app_session_key);
    fake_data(gs.lora.network_session_key);
    fake_data(gs.lora.device_address);

    for (auto i = 0u; i < WifiMaximumNumberOfNetworks; ++i) {
        fake_string(gs.network.config.wifi_networks[i].ssid);
        fake_string(gs.network.config.wifi_networks[i].password);
    }

    gs.scheduler.readings.interval = 120;
    gs.scheduler.network.interval = 120;
    gs.scheduler.gps.interval = 3600;
    gs.scheduler.lora.interval = 86400;

    auto module_readings = new (pool) NModuleReadings<10>();
    module_readings->set(0, 23.0f);
    module_readings->set(1, 332.0f);
    module_readings->set(2, 934839.0f);
    module_readings->set(3, 100.0f);
    module_readings->set(4, 39843.0f);
    module_readings->set(5, -23.0f);
    module_readings->set(6, 825398.0f);
    module_readings->set(7, 41017.0f);
    module_readings->set(8, 2111193.0f);
    module_readings->set(9, 937281.0f);

    ModuleReadingsCollection readings{ pool };
    readings.emplace(ModuleMetaAndReadings{
        .position = 0,
        .id = nullptr,
        .meta = &fk_test_module_fake_empty,
        .sensors = nullptr,
        .readings = module_readings,
    });

    fk_uuid_t module_id;
    fake_data(module_id.data);

    auto module_sensors = pool.malloc_with<SensorState, 10>({
        { .name = "sensor-0", .unit_of_measure = "m", .flags = 0, .has_live_vaue = true, .live_value = module_readings->get(0) },
        { .name = "sensor-1", .unit_of_measure = "m", .flags = 0, .has_live_vaue = true, .live_value = module_readings->get(1) },
        { .name = "sensor-2", .unit_of_measure = "m", .flags = 0, .has_live_vaue = true, .live_value = module_readings->get(2) },
        { .name = "sensor-3", .unit_of_measure = "m", .flags = 0, .has_live_vaue = true, .live_value = module_readings->get(3) },
        { .name = "sensor-4", .unit_of_measure = "m", .flags = 0, .has_live_vaue = true, .live_value = module_readings->get(4) },
        { .name = "sensor-5", .unit_of_measure = "m", .flags = 0, .has_live_vaue = true, .live_value = module_readings->get(5) },
        { .name = "sensor-6", .unit_of_measure = "m", .flags = 0, .has_live_vaue = true, .live_value = module_readings->get(6) },
        { .name = "sensor-7", .unit_of_measure = "m", .flags = 0, .has_live_vaue = true, .live_value = module_readings->get(7) },
        { .name = "sensor-8", .unit_of_measure = "m", .flags = 0, .has_live_vaue = true, .live_value = module_readings->get(8) },
        { .name = "sensor-9", .unit_of_measure = "m", .flags = 0, .has_live_vaue = true, .live_value = module_readings->get(9) },
    });

    auto module_states = pool.malloc_with<ModuleState, 1>({
        {
            .position = 0,
            .manufacturer = 1,
            .kind = 2,
            .version = 3,
            .name = "module",
            .display_name_key = "module",
            .id = (fk_uuid_t *)pool.copy(&module_id, sizeof(fk_uuid_t)),
            .flags = 0,
            .sensors = module_sensors,
            .nsensors = 10,
        }
    });

    auto modules = new (pool) ModulesState(&pool);
    modules->nmodules = 1;
    modules->modules = module_states;
    modules->readings_time = 65536;
    modules->readings_number = 32768;

    gs.modules = modules;
}

static void fake_modules(ConstructedModulesCollection &modules, Pool &pool) {
    modules.emplace(ConstructedModule{
        .found = { },
        .meta = &fk_test_module_fake_1,
        .module = fk_test_module_fake_1.ctor(pool),
    });
    modules.emplace(ConstructedModule{
        .found = { },
        .meta = &fk_test_module_fake_2,
        .module = fk_test_module_fake_2.ctor(pool),
    });
    modules.emplace(ConstructedModule{
        .found = { },
        .meta = &fk_test_module_fake_1,
        .module = fk_test_module_fake_1.ctor(pool),
    });
    modules.emplace(ConstructedModule{
        .found = { },
        .meta = &fk_test_module_fake_2,
        .module = fk_test_module_fake_2.ctor(pool),
    });

    for (auto &m : modules) {
        fake_data(m.found.header.id.data);
    }
}

static void dump_binary(std::ostream &stream, std::string prefix, EncodedMessage *message) {
    stream << prefix << " ";
    for (auto i = 0u; i < message->size; ++i) {
        stream << std::setfill('0') << std::setw(2) << std::hex << (int32_t)message->buffer[i] << " ";
        if ((i + 1) % 32 == 0) {
            if (i + 1 < message->size) {
                stream << std::endl << prefix << " ";
            }
        }
    }
    stream << std::endl;
}

class ProtoBufSizeSuite : public ::testing::Test {
protected:
    static std::ofstream file_;
    StandardPool pool_{ "storage" };

public:
    ProtoBufSizeSuite() {
        if (!file_.is_open()) {
            file_.open("protobuf.pb");
        }
    }

    void SetUp() override {
        pool_.clear();

        fake_string(fake_header.firmware.version);
        fake_string(fake_header.firmware.name);
        fake_string(fake_header.firmware.hash);
    }

    void TearDown() override {
    }

};

std::ofstream ProtoBufSizeSuite::file_;

TEST_F(ProtoBufSizeSuite, Readings) {
    GlobalState gs;
    fake_global_state(gs, pool_);

    TwoWireWrapper module_bus{ "modules", nullptr };
    ScanningContext ctx{ get_modmux(), &gs, module_bus, pool_ };
    ConstructedModulesCollection resolved(pool_);
    fake_modules(resolved, pool_);

    Readings readings{ get_modmux() };
    ASSERT_TRUE(readings.take_readings(ctx, resolved, 1, 1, pool_));

    auto encoded = pool_.encode(fk_data_DataRecord_fields, &readings.record());
    dump_binary(file_, "data-readings", encoded);

    ASSERT_EQ(encoded->size, 218u);
}

TEST_F(ProtoBufSizeSuite, Configuration) {
    GlobalState gs;
    fake_global_state(gs, pool_);

    MetaRecord record;
    record.include_state(&gs, &fake_header, pool_);

    auto encoded = pool_.encode(fk_data_DataRecord_fields, &record.record());
    dump_binary(file_, "data-configuration", encoded);

    ASSERT_EQ(encoded->size, 1359u);
}

TEST_F(ProtoBufSizeSuite, Modules) {
    ConstructedModulesCollection resolved(pool_);
    fake_modules(resolved, pool_);

    GlobalState gs;
    fake_global_state(gs, pool_);

    MetaRecord record;
    record.include_modules(&gs, &fake_header, resolved, pool_);

    auto encoded = pool_.encode(fk_data_DataRecord_fields, &record.record());
    dump_binary(file_, "data-modules", encoded);

    ASSERT_EQ(encoded->size, 934u);
}

TEST_F(ProtoBufSizeSuite, HttpReplyStatus) {
    GlobalState gs;
    fake_global_state(gs, pool_);

    HttpReply reply(pool_, &gs);
    reply.include_status(1580763366, 327638, &fake_header);

    auto encoded = pool_.encode(fk_app_HttpReply_fields, reply.reply());
    dump_binary(file_, "http-reply-status", encoded);

    ASSERT_EQ(encoded->size, 1638u);
}

TEST_F(ProtoBufSizeSuite, HttpReplyReadings) {
    GlobalState gs;
    fake_global_state(gs, pool_);

    HttpReply reply(pool_, &gs);
    reply.include_readings();

    auto encoded = pool_.encode(fk_app_HttpReply_fields, reply.reply());
    dump_binary(file_, "http-reply-readings", encoded);

    ASSERT_EQ(encoded->size, 292u);
}
