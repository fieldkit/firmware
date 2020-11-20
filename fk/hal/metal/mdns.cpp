#include "platform.h"
#include "hal/metal/mdns.h"
#include "hal/metal/mdns_pool_allocator.h"
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

FK_DECLARE_LOGGER("mdns");

MDNSDiscovery::MDNSDiscovery() {
}

MDNSDiscovery::~MDNSDiscovery() {
}

bool MDNSDiscovery::start() {
    FK_ASSERT(pool_ != nullptr);

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

    if (!mdns_.addServiceRecord(service_name_, 80, MDNSServiceTCP)) {
        logerror("error adding service");
        return false;
    }

    registered_ = fk_uptime();

    initialized_ = true;

    loginfo("ready!");

    return true;
}

bool MDNSDiscovery::service(Pool *pool) {
    if (false) {
        if (fk_uptime() - registered_ > NetworkAddServiceRecordIntervalMs) {
            if (!mdns_.addServiceRecord(service_name_, 80, MDNSServiceTCP)) {
                logwarn("error adding service");
            }

            registered_ = fk_uptime();
        }
    }

    udp_.dns_pool(pool);

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

/*
auto try_again = false;
auto discover_service = "_fk";

static void service_found(const char *type, MDNSServiceProtocol, const char *name, IPAddress ip, unsigned short port, const char *txt) {
    if (nullptr == name) {
        loginfo("discovery finished: none");
    } else {
        loginfo("discovery finished: %s", name);

        // Check out http://www.zeroconf.org/Rendezvous/txtrecords.html for a
        // primer on the structure of TXT records. Note that the Bonjour
        // library will always return the txt content as a zero-terminated
        // string, even if the specification does not require this.
        if (nullptr != txt) {
            char buf[256];
            char len = *txt++;
            while (len) {
                auto i = 0;
                while (len--) {
                    buf[i++] = *txt++;
                }
                buf[i] = '\0';
                loginfo("txt: %s", buf);
                len = *txt++;
            }
        }
    }

    try_again = true;
}

// Initialize
if (false) {
    mdns_.setServiceFoundCallback(service_found);

    if (!mdns_.startDiscoveringService(discover_service, MDNSServiceTCP, 5000)) {
        logwarn("error starting discovery");
    }
}
*/
