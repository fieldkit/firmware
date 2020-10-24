#include "platform.h"
#include "hal/metal/mdns.h"
#include "common.h"
#include "utilities.h"
#include "config.h"

namespace fk {

/**
 * HACK: Hacks around amplifying service registration traffic.
 */
constexpr uint32_t NetworkAddServiceRecordIntervalMs = 5000;
constexpr uint32_t NetworkRemoveServiceRecordAmplification = 5;
constexpr uint32_t NetworkRemoveServiceRecordDelayMs = 250;

class MDNSPoolAllocator : public MDNSAllocator {
private:
    Pool *pool_;

public:
    explicit MDNSPoolAllocator(Pool *pool) : pool_(pool) {
    }

public:
    void *malloc(size_t size) override {
        return pool_->calloc(size);
    }

    void free(void *ptr) override {
    }

};

FK_DECLARE_LOGGER("mdns");

MDNSDiscovery::MDNSDiscovery() {
}

MDNSDiscovery::~MDNSDiscovery() {
}

bool MDNSDiscovery::start(Pool *pool) {
    FK_ASSERT(pool != nullptr);

    pool_ = pool;

    loginfo("starting");

    if (initialized_) {
        return true;
    }

    IPAddress ip = WiFi.localIP();

    fk_serial_number_t sn;
    const char *suffix = "._fk";
    bytes_to_hex_string(name_, sizeof(name_), (uint8_t *)&sn, sizeof(sn));
    auto id_length = bytes_to_hex_string(service_name_, sizeof(service_name_) - strlen(suffix) - 1, (uint8_t *)&sn, sizeof(sn));
    strncpy(service_name_ + id_length, suffix, strlen(suffix));
    service_name_[id_length + 4] = 0;

    mdns_.allocator(new (pool_) MDNSPoolAllocator(pool_));

    if (!mdns_.begin(ip, name_)) {
        logerror("unable to start mdns responder!");
        return false;
    }

    if (false) {
        if (!mdns_.addServiceRecord(service_name_, 80, MDNSServiceTCP)) {
            logerror("error adding service");
            return false;
        }
    }

    registered_ = fk_uptime();

    initialized_ = true;

    loginfo("ready!");

    return true;
}

bool MDNSDiscovery::service() {
    if (false) {
        if (fk_uptime() - registered_ > NetworkAddServiceRecordIntervalMs) {
            if (!mdns_.addServiceRecord(service_name_, 80, MDNSServiceTCP)) {
                logwarn("error adding service");
            }

            registered_ = fk_uptime();
        }
    }

    mdns_.run();

    /*
    if (try_again) {
        if (!mdns_.startDiscoveringService(discover_service, MDNSServiceTCP, 5000)) {
            logwarn("error starting discovery");
        }
        try_again = false;
    }
    */
    return true;
}

void MDNSDiscovery::stop() {
    if (!initialized_) {
        return;
    }

    mdns_.removeServiceRecord(80, MDNSServiceTCP, NetworkRemoveServiceRecordAmplification,
                              NetworkRemoveServiceRecordDelayMs);
    udp_.stop();

    initialized_ = false;
}

} // namespace fk
