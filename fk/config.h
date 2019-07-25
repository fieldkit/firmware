#pragma once

#include <inttypes.h>
#include <string.h>

namespace fk {

/**
 * Maximum number of memory banks we're capable of supporting.
 */
#define FK_MAXIMUM_NUMBER_OF_MEMORY_BANKS (2)

/**
 * Maximum number of memory banks we're capable of supporting.
 */
constexpr size_t MaximumNumberOfMemoryBanks = FK_MAXIMUM_NUMBER_OF_MEMORY_BANKS;

/**
 * One second in milliseconds.
 */
constexpr uint32_t OneSecondMs = 1000;

/**
 * Two seconds in milliseconds.
 */
constexpr uint32_t TwoSecondsMs = 2000;

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
 * Precisely 1024 * 1024.
 */
constexpr uint32_t OneMegabyte = 1024 * 1024;

/**
 * How long the user should hold the button to initiate a factory wipe.
 */
constexpr uint32_t FactoryWipeButtonDuration = TwoSecondsMs;

/**
 * This is the theorhetical maximum number of modules that can be physically
 * connected to a single station. This is governed by the largest backplanes and
 * their possible combinations.
 */
constexpr size_t MaximumNumberOfModules = 8;

/**
 * The number of fixed HTTP routes that the firmware registers. These are routes
 * that are always there and never change with configuration.
 */
constexpr size_t HttpFixedRoutes = 2;

/**
 * The maximum number of HTTP routes that can be registered. Trying to register
 * more than this will fail. This is a function of the maximum number of modules
 * and the number of fixed routes.
 */
constexpr size_t HttpMaximumRoutes = MaximumNumberOfModules + HttpFixedRoutes;

/**
 * The maximum length of an incoming URL.
 */
constexpr size_t HttpdMaximumUrlLength = 64;

/**
 * How long to wait for a WiFi connection to establish.
 */
constexpr uint32_t WifiConnectionTimeoutMs = 30 * 1000;

/**
 * Buffer size for storing GPS sentences for debugging purposes.
 */
constexpr size_t GpsDebuggingBuffer = 64;

/**
 * Maximum size of all the headers in an HTTP request.
 */
constexpr size_t HttpMaximumHeaderSize = 1024;

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
     * Logging configuration.
     */
    typedef struct logging_t {
        /**
         * Log raw GPS information, very verbose.
         */
        bool gps_raw{ false };
    } logging_t;

    /**
     * Logging configuration.
     */
    logging_t logging;

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
