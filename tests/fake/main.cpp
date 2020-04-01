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

#include "modules/registry.h"
#include "modules/module_factory.h"

#include "readings_taker.h"
#include "test_modules.h"
#include "state_ref.h"
#include "memory.h"

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

    fk_modules_builtin_register(&fk_test_module_fake_random);
    fk_modules_builtin_register(&fk_test_module_fake_diagnostics);

    FK_ASSERT(memory->begin());

    StandardPool pool{ "fake" };
    Storage storage{ memory, pool, false };
    FK_ASSERT(storage.clear());

    TwoWireWrapper module_bus{ "modules", nullptr };
    ScanningContext ctx{ get_modmux(), get_global_state_rw().get(), module_bus };

    for (size_t i = 0; i < 1000; ++i) {
        StandardPool pool{ "readings" };
        FoundModuleCollection found(pool);
        found.emplace(FoundModule{
                .position = 0xff,
                .header = {
                    .manufacturer = FK_MODULES_MANUFACTURER,
                    .kind = FK_MODULES_KIND_RANDOM,
                    .version = 0x01,
                }
            });

        StaticModuleScanning scanning(found);
        ModuleFactory module_factory;
        auto constructed_maybe = module_factory.rescan_and_initialize(ctx, scanning, pool);

        ReadingsTaker readings_taker{ storage, get_modmux(), false };

        loginfo("writing fake data");

        FK_ASSERT(readings_taker.take(*constructed_maybe, ctx, pool));
    }

    loginfo("done");
}

static void server(Fake *fake) {
    StandardPool pool{ "pool" };
    LinuxNetwork network;
    NetworkServices network_services{ &network };
    auto gs = get_global_state_ro();

    auto settings = NetworkSettings{
        .valid = true,
    };
    if (!network_services.begin(settings, FiveMinutesMs, pool)) {
        return;
    }

    loginfo("serving...");

    if (!network_services.serve()) {
        return;
    }

    while (fake->running()) {
        network_services.tick();
    }

    loginfo("stopping...");

    network_services.stop();
}

__int32_t main(__int32_t argc, const char **argv) {
    log_configure_level(LogLevels::INFO);
    fk_standard_page_initialize();

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

extern "C" {

void osi_assert(const char *assertion, const char *file, int line) {
    fprintf(stderr, "\n\nassertion \"%s\" failed: file \"%s\", line %d\n", assertion, file, line);
    exit(2);
}

void fk_assert(const char *assertion, const char *file, int32_t line, const char *f, ...) {
    fprintf(stderr, "\n\nassertion \"%s\" failed: file \"%s\", line %d\n", assertion, file, line);
    exit(2);
}

}
