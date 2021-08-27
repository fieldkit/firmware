#include <fstream>

#include <fk-data-protocol.h>

#include "hal/hal.h"
#include "networking/http_reply.h"
#include "storage/meta_record.h"
#include "storage/signed_log.h"
#include "update_readings_listener.h"

#include "storage_suite.h"

#include "test_modules.h"

using namespace fk;

static fkb_header_t fake_header = { .signature = { 'F', 'K', 'B', 0 },
                                    .version = 1,
                                    .size = sizeof(fkb_header_t),
                                    .firmware = { .flags = 0,
                                                  .timestamp = 1580763366,
                                                  .number = 1000,
                                                  .reserved = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff },
                                                  .safe = 0xff,
                                                  .previous = UINT32_MAX,
                                                  .binary_size = 65536,
                                                  .tables_offset = 8192,
                                                  .data_size = 8192,
                                                  .bss_size = 8192,
                                                  .got_size = 8192,
                                                  .vtor_offset = 8192,
                                                  .got_offset = 32768,
                                                  .version = { 0x0 },
                                                  .hash_size = 32,
                                                  .hash = { 0xB2 } },
                                    .number_symbols = 100,
                                    .number_relocations = 100 };

FK_DECLARE_LOGGER("protobuf");

template <typename T = uint8_t, size_t Size> static void fake_data(T (&buffer)[Size]) {
    for (auto i = 0u; i < Size; ++i) {
        buffer[i] = i % 255;
    }
}

template <typename T = char, size_t Size> static void fake_string(T (&buffer)[Size]) {
    for (auto i = 0u; i < Size - 1; ++i) {
        buffer[i] = 'a' + (i % 10);
    }
    buffer[Size - 1] = 0;
}

static SensorMetadata const fk_module_fake_3_sensor_metas[] = {
    { .name = "sensor-0", .unitOfMeasure = "m", .flags = 0 }, { .name = "sensor-1", .unitOfMeasure = "m", .flags = 0 },
    { .name = "sensor-2", .unitOfMeasure = "m", .flags = 0 }, { .name = "sensor-3", .unitOfMeasure = "m", .flags = 0 },
    { .name = "sensor-4", .unitOfMeasure = "m", .flags = 0 }, { .name = "sensor-5", .unitOfMeasure = "m", .flags = 0 },
    { .name = "sensor-6", .unitOfMeasure = "m", .flags = 0 }, { .name = "sensor-7", .unitOfMeasure = "m", .flags = 0 },
    { .name = "sensor-8", .unitOfMeasure = "m", .flags = 0 }, { .name = "sensor-9", .unitOfMeasure = "m", .flags = 0 },
};

static ModuleSensors fk_module_fake_3_sensors = {
    .nsensors = sizeof(fk_module_fake_3_sensor_metas) / sizeof(SensorMetadata),
    .sensors = fk_module_fake_3_sensor_metas,
};

class FakeModule3 : public FakeModule {
public:
    ModuleReturn initialize(ModuleContext mc, Pool &pool) override {
        return { ModuleStatus::Ok };
    }

    ModuleReturn api(ModuleContext mc, HttpServerConnection *connection, Pool &pool) {
        return { ModuleStatus::Ok };
    }

    ModuleReturn service(ModuleContext mc, Pool &pool) {
        return { ModuleStatus::Ok };
    }

    ModuleSensors const *get_sensors(Pool &pool) override {
        return &fk_module_fake_3_sensors;
    }

    ModuleConfiguration const get_configuration(Pool &pool) override {
        return { "module" };
    }

    ModuleReadings *take_readings(ReadingsContext mc, Pool &pool) override {
        auto mr = new (pool) NModuleReadings<10>();
        for (size_t i = 0; i < mr->size(); i++) {
            mr->set(i, (float)fk_random_i32(20, 100));
        }
        return mr;
    }
};

static Module *fk_test_module_create_3(Pool &pool) {
    return new (pool) FakeModule3();
}

ModuleMetadata const fk_test_module_fake_3 = {
    .manufacturer = 1,
    .kind = 2,
    .version = 3,
    .name = "module",
    .flags = FK_MODULES_FLAG_NONE,
    .ctor = fk_test_module_create_3,
};

static void fake_global_state(GlobalState &gs, Pool &pool) {
    gs.version = 100;
    fake_string(gs.general.name);
    fake_data(gs.general.generation);

    gs.general.recording = 1580763366;

    // gs.lora.configured = true;
    // gs.lora.uplink_counter = 4096;
    // gs.lora.downlink_counter = 1024;
    fake_data(gs.lora.device_eui);
    fake_data(gs.lora.join_eui);
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

    state::DynamicState dynamic;

    ModuleHeader header = {
        .manufacturer = 1,
        .kind = 2,
        .version = 3,
    };
    fake_data(header.id.data);

    auto attached = dynamic.attached();
    auto position = ModulePosition::from(0);
    attached->modules().emplace(position, header, &fk_test_module_fake_3, fk_test_module_fake_3.ctor(pool), pool);

    NoopMutex mutex;
    TwoWireWrapper module_bus{ &mutex, "modules", nullptr };
    ScanningContext ctx{ get_modmux(), gs.location(pool), module_bus, pool };
    for (auto &attached_module : attached->modules()) {
        auto sub_ctx = ctx.open_module(attached_module.position(), pool);
        attached_module.initialize(sub_ctx, &pool);
    }

    float values[] = { 23.0f, 332.0f, 934839.0f, 100.0f, 39843.0f, -23.0f, 825398.0f, 41017.0f, 2111193.0f, 937281.0f };

    auto attached_module = attached->get_by_position(position);
    auto &sensors = attached_module->sensors();
    for (auto &sensor : sensors) {
        sensor.reading(ModuleReading{ values[sensor.index()] });
    }

    gs.dynamic = std::move(dynamic);
}

static void fake_modules(GlobalState &gs, Pool &pool) {
    state::DynamicState dynamic;

    auto attached = dynamic.attached();

    ModuleHeader header;
    fake_data(header.id.data);
    attached->modules().emplace(ModulePosition::from(0), header, &fk_test_module_fake_1, fk_test_module_fake_1.ctor(pool), pool);

    fake_data(header.id.data);
    attached->modules().emplace(ModulePosition::from(1), header, &fk_test_module_fake_2, fk_test_module_fake_2.ctor(pool), pool);

    fake_data(header.id.data);
    attached->modules().emplace(ModulePosition::from(2), header, &fk_test_module_fake_1, fk_test_module_fake_1.ctor(pool), pool);

    fake_data(header.id.data);
    attached->modules().emplace(ModulePosition::from(3), header, &fk_test_module_fake_2, fk_test_module_fake_2.ctor(pool), pool);

    NoopMutex mutex;
    TwoWireWrapper module_bus{ &mutex, "modules", nullptr };
    ScanningContext ctx{ get_modmux(), gs.location(pool), module_bus, pool };
    for (auto &attached_module : attached->modules()) {
        auto sub_ctx = ctx.open_module(attached_module.position(), pool);
        attached_module.initialize(sub_ctx, &pool);
    }

    UpdateReadingsListener listener{ pool };
    attached->take_readings(&listener, pool);
    listener.flush();

    gs.dynamic = std::move(dynamic);
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
        fake_string(fake_header.firmware.hash);
    }

    void TearDown() override {
    }
};

std::ofstream ProtoBufSizeSuite::file_;

TEST_F(ProtoBufSizeSuite, Readings) {
    GlobalState gs;
    fake_global_state(gs, pool_);
    fake_modules(gs, pool_);

    gs.readings.nreadings = 1;

    DataRecord record{ pool_ };
    record.include_readings(&gs, &fake_header, 1, pool_);

    record.record().readings.uptime = 1;

    auto encoded = pool_.encode(fk_data_DataRecord_fields, &record.record());
    dump_binary(file_, "data-readings", encoded);

    ASSERT_EQ(encoded->size, 221u);
}

TEST_F(ProtoBufSizeSuite, ReadingsNoneBackFromFirstModule) {
    GlobalState gs;
    fake_global_state(gs, pool_);

    NoopMutex mutex;
    TwoWireWrapper module_bus{ &mutex, "modules", nullptr };
    ScanningContext ctx{ get_modmux(), gs.location(pool_), module_bus, pool_ };

    auto first = (FakeModule1 *)fk_test_module_fake_1.ctor(pool_);
    first->return_none();

    state::DynamicState dynamic;
    auto attached = dynamic.attached();

    ModuleHeader header;
    fake_data(header.id.data);
    attached->modules().emplace(ModulePosition::from(0), header, &fk_test_module_fake_1, first, pool_);
    fake_data(header.id.data);
    attached->modules().emplace(ModulePosition::from(1), header, &fk_test_module_fake_2, fk_test_module_fake_2.ctor(pool_), pool_);

    attached->initialize(pool_);

    UpdateReadingsListener listener{ pool_ };
    attached->take_readings(&listener, pool_);
    listener.flush();

    gs.dynamic = std::move(dynamic);
    gs.readings.nreadings = 1;

    // This is failing because we actually serialize all sensors now,
    // regardless of what a readings take does. That'll have to change.

    DataRecord record{ pool_ };
    record.include_readings(&gs, &fake_header, 1, pool_);

    record.record().readings.uptime = 1;

    auto encoded = pool_.encode(fk_data_DataRecord_fields, &record.record());
    dump_binary(file_, "data-readings-failed-first", encoded);

    ASSERT_EQ(encoded->size, 111u);
}

TEST_F(ProtoBufSizeSuite, Configuration) {
    GlobalState gs;
    fake_global_state(gs, pool_);

    MetaRecord record{ pool_ };
    record.include_state(&gs, &fake_header, pool_);

    auto encoded = pool_.encode(fk_data_DataRecord_fields, record.record());
    dump_binary(file_, "data-configuration", encoded);

    ASSERT_EQ(encoded->size, 1256u);
}

TEST_F(ProtoBufSizeSuite, Modules) {
    GlobalState gs;
    fake_global_state(gs, pool_);
    fake_modules(gs, pool_);

    MetaRecord record{ pool_ };
    record.include_modules(&gs, &fake_header, pool_);

    auto encoded = pool_.encode(fk_data_DataRecord_fields, record.record());
    dump_binary(file_, "data-modules", encoded);

    ASSERT_EQ(encoded->size, 957u);
}

TEST_F(ProtoBufSizeSuite, HttpReplyStatus) {
    GlobalState gs;
    fake_global_state(gs, pool_);

    HttpReply reply(pool_, &gs);
    reply.include_status(1580763366, 327638, true, &fake_header);

    auto encoded = pool_.encode(fk_app_HttpReply_fields, reply.reply());
    dump_binary(file_, "http-reply-status", encoded);

    ASSERT_EQ(encoded->size, 2043u);
}

TEST_F(ProtoBufSizeSuite, HttpReplyReadings) {
    GlobalState gs;
    fake_global_state(gs, pool_);

    HttpReply reply(pool_, &gs);
    reply.include_readings();

    auto encoded = pool_.encode(fk_app_HttpReply_fields, reply.reply());
    dump_binary(file_, "http-reply-readings", encoded);

    ASSERT_EQ(encoded->size, 342u);
}
