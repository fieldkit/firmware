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

#include "registry.h"
#include "readings_taker.h"
#include "test_modules.h"

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

static void setup_fake_data() {
    auto memory = MemoryFactory::get_data_memory();

    fk_modules_builtin_register(&fk_test_module_fake_empty);
    fk_modules_builtin_register(&fk_test_module_fake_1);
    fk_modules_builtin_register(&fk_test_module_fake_2);

    FK_ASSERT(memory->begin());

    Storage storage{ memory };
    FK_ASSERT(storage.clear());

    TwoWireWrapper module_bus{ "modules", nullptr };
    ModuleContext mc{ get_global_state_rw().get(), module_bus };

    for (auto i = 0; i < 10000 / FK_READINGS_AMPLIFY_WRITES; ++i) {
        MallocPool pool{ "readings", 2048 };
        FoundModuleCollection found(pool);
        found.emplace_back(FoundModule{
                .position = 0xff,
                .header = {
                    .manufacturer = FK_MODULES_MANUFACTURER,
                    .kind = FK_MODULES_KIND_RANDOM,
                    .version = 0x01,
                }
            });

        StaticModuleScanning scanning(found);
        ReadingsTaker readings_taker{ scanning, storage, get_modmux(), false };

        loginfo("writing fake data");

        FK_ASSERT(readings_taker.take(mc, pool));
    }

    loginfo("done");
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

    log_configure_level(LogLevels::DEBUG);

    loginfo("starting thread...");

    std::thread server_thread(server, &fake);

    fake.run();

    server_thread.join();

    loginfo("done");

    return 0;
}
