#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <inttypes.h>
#include <signal.h>
#include <unistd.h>
#include <thread>

#include "fk.h"
#include "hal/linux/linux.h"
#include "httpd/httpd.h"

FK_DECLARE_LOGGER("main");

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

void server(Fake *fake) {
    fk::LinuxNetwork network;
    fk::HttpServer http_server{ &network };

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
    std::thread server_thread(server, &fake);

    log_configure_level(LogLevels::TRACE);

    loginfo("hello");

    struct sigaction si_handler;
    si_handler.sa_handler = signal_handler;
    sigemptyset(&si_handler.sa_mask);
    si_handler.sa_flags = 0;
    sigaction(SIGINT, &si_handler, NULL);

    fake.run();

    server_thread.join();

    loginfo("done");

    return 0;
}
