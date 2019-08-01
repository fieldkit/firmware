#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <inttypes.h>
#include <signal.h>
#include <unistd.h>
#include <thread>

#include <fk-data-protocol.h>

#include "fk.h"
#include "hal/linux/linux.h"
#include "networking/networking.h"
#include "protobuf.h"

FK_DECLARE_LOGGER("main");

using namespace fk;

class Fake {
private:
    bool running_{ false };

public:
    Fake() {
    }

public:
    bool running() {
        return running_;
    }

    void run() {
        running_ = true;
    }

    void stop() {
        running_ = false;
    }

};

Fake fake;

static void signal_handler(int32_t s){
    fprintf(stderr, "\n");
    loginfo("signal!");
    fake.stop();
}

static size_t write_reading(File &file) {
    fk_data_SensorAndValue readings[] = {
        { 0, (float)random() },
        { 1, (float)random() },
        { 2, (float)random() },
        { 3, (float)random() },
        { 4, (float)random() },
        { 5, (float)random() },
        { 6, (float)random() },
        { 7, (float)random() },
        { 8, (float)random() },
        { 9, (float)random() },
    };

    pb_array_t readings_array = {
        .length = (size_t)10,
        .itemSize = sizeof(fk_data_SensorAndValue),
        .buffer = &readings,
        .fields = fk_data_SensorAndValue_fields,
    };

    fk_data_SensorGroup groups[] = {
        {
            .module = 0,
            .readings = {
                .funcs = {
                    .encode = pb_encode_array,
                },
                .arg = &readings_array,
            },
        }
    };

    pb_array_t sensor_groups_array = {
        .length = (size_t)1,
        .itemSize = sizeof(fk_data_SensorGroup),
        .buffer = &groups,
        .fields = fk_data_SensorGroup_fields,
    };

    fk_data_DataRecord record = fk_data_DataRecord_init_default;
    record.readings.time = fk_uptime();
    record.readings.reading = file.record();
    record.readings.flags = 0;
    record.readings.location.fix = 0;
    record.readings.location.time = fk_uptime();
    record.readings.location.longitude = -118.2709223;
    record.readings.location.latitude = 34.0318047;
    record.readings.location.altitude = 100.0f;
    record.readings.sensorGroups.funcs.encode = pb_encode_array;
    record.readings.sensorGroups.arg = &sensor_groups_array;

    auto wrote = file.write(&record, fk_data_DataRecord_fields);
    if (wrote == 0) {
        logerror("error saving readings");
        return 0;
    }

    return wrote;
}

static void setup_fake_data() {
    auto memory = MemoryFactory::get_data_memory();

    FK_ASSERT(memory->begin());

    Storage storage{ memory };

    loginfo("writing fake data");

    auto file = storage.file(0);
    auto size = 0;

    for (auto i = 0; i < 1000; ++i) {
        auto wrote = write_reading(file);
        FK_ASSERT(wrote > 0);
        size += wrote;
    }

    loginfo("done (%d)", size);
}

static void server(Fake *fake) {
    configuration_t fkc;
    LinuxNetwork network;
    HttpServer http_server{ &network, &fkc };

    if (!http_server.begin()) {
        return;
    }

    loginfo("serving...");

    while (fake->running()) {
        http_server.tick();
    }

    loginfo("stopping...");

    http_server.stop();
}

__int32_t main(__int32_t argc, const char **argv) {
    log_configure_level(LogLevels::DEBUG);

    loginfo("hello");

    setup_fake_data();

    struct sigaction si_handler;
    si_handler.sa_handler = signal_handler;
    sigemptyset(&si_handler.sa_mask);
    si_handler.sa_flags = 0;
    sigaction(SIGINT, &si_handler, NULL);

    loginfo("starting thread...");

    std::thread server_thread(server, &fake);

    fake.run();

    server_thread.join();

    loginfo("done");

    return 0;
}
