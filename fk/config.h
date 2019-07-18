#pragma once

#include <inttypes.h>

namespace fk {

/**
 * One second in milliseconds.
 */
constexpr uint32_t OneSecondMs = 1000;

/**
 * Five seconds in milliseconds.
 */
constexpr uint32_t FiveSecondsMs = 5000;

/**
 * Ten seconds in milliseconds.
 */
constexpr uint32_t TenSecondsMs = 10 * 1000;

/**
 * Thirty seconds in milliseconds.
 */
constexpr uint32_t ThirtySecondsMs = 30 * 1000;

/**
 * One minute in milliseconds.
 */
constexpr uint32_t OneMinuteMs = 60 * 1000;

/**
 * Five minutes in milliseconds.
 */
constexpr uint32_t FiveMinutesMs = 5 * 60 * 1000;

/**
 * Maximum number of WiFi networks.
 * \todo: Eventually this should be relaxed.
 */
constexpr uint32_t MaximumNumberOfWifiNetworks = 2;

/**
 * Runtime configuration informationthat is unavailable in the mobile
 * application. This can be updated after compilation to build binaries for
 * various situations.
 */
typedef struct configuration_t {
    /**
     * Enables showing the logos during startup, this induces a longer delay and
     * so can be turned off to speed up testing/development.
     */
    bool slow_startup{ false };

    /**
     * Run a full self check, even for things that don't necessarily need to be
     * working. This affects checking GPS and SD Card.
     */
    bool full_self_check{ false };

    /**
     * Configuration for an individual WiFi network.
     */
    typedef struct wifi_network_t {
        /**
         * SSID for the network.
         */
        const char *ssid;

        /**
         * Password for the network.
         */
        const char *password;
    } wifi_network_t;

    /**
     * Network related configuration.
     */
    typedef struct network_t {
        /**
         * How long network is kept up and available before turning off due to inactivity.
         */
        uint32_t uptime{ FiveMinutesMs };

        /**
         * WiFi network configurations.
         */
        wifi_network_t networks[MaximumNumberOfWifiNetworks] = {
            #if defined(FK_WIFI_0_SSID) && defined(FK_WIFI_0_PASSWORD)
            { FK_WIFI_0_SSID, FK_WIFI_0_PASSWORD },
            #else
            { nullptr, nullptr },
            #endif
            #if defined(FK_WIFI_1_SSID) && defined(FK_WIFI_1_PASSWORD)
            { FK_WIFI_1_SSID, FK_WIFI_1_PASSWORD },
            #else
            { nullptr, nullptr },
            #endif
        };
    } network_t;

    /**
     * Network related configuration.
     */
    network_t network;
} configuration_t;

/**
 * Singleton instance of the runtime configuration data.
 */
extern configuration_t fkc;

}
