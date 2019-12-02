#pragma once

#include <inttypes.h>
#include <string.h>

#include "secrets.h"

namespace fk {

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
 * Five minutes in seconds.
 */
constexpr uint32_t FiveMinutesSeconds = 5 * 60;

/**
 * Thirty minutes in milliseconds.
 */
constexpr uint32_t ThirtyMinutesMs  = OneMinuteMs * 30;

/**
 * One hour in milliseconds.
 */
constexpr uint32_t OneHourMs = OneMinuteMs * 60;

/**
 * One day in milliseconds.
 */
constexpr uint32_t OneDayMs = OneHourMs * 24;

/**
 * One day in seconds.
 */
constexpr uint32_t OneDaySeconds  = 86400;

/**
 * Precisely 1024 * 1024.
 */
constexpr uint32_t OneMegabyte = 1024 * 1024;

/**
 * How often to save LoRa radio state.
 */
constexpr uint32_t LoraUplinksSaveFrequency = 10;

/**
 * Length of a LoRa App EUI.
 */
constexpr size_t LoraAppEuiLength = 8;

/**
 * Length of a LoRa Device EUI.
 */
constexpr size_t LoraDeviceEuiLength = 8;

/**
 * Length of a LoRa App Key.
 */
constexpr size_t LoraAppKeyLength = 16;

/**
 * Length of a LoRa Device Address.
 */
constexpr size_t LoraDeviceAddressLength = 4;

/**
 * Length of a LoRa network session key.
 */
constexpr size_t LoraNetworkSessionKeyLength = 16;

/**
 * Length of a LoRa app session key.
 */
constexpr size_t LoraAppSessionKeyLength = 16;

/**
 * LoRaWAN port for status messages.
 */
constexpr uint8_t LoraStatusPort = 11;

/**
 * LoRaWAN port for data messages.
 */
constexpr uint8_t LoraDataPort = 10;

/**
 * Maximum length of a device's name.
 */
constexpr size_t MaximumNameLength = 64;

/**
 * Maximum length of the build string.
 */
constexpr size_t MaximumBuildStringLength = 32;

/**
 * Maximum length of a device's generation. This is regeneration whenever the
 * file system is created.
 */
constexpr size_t GenerationLength = 32;

/**
 * Size of the memory pool to use for Workers.
 */
constexpr size_t DefaultWorkerPoolSize = 8192;

/**
 * Maximum number of WiFi networks.
 * \todo: Eventually this should be relaxed.
 */
constexpr size_t MaximumNumberOfWifiNetworks = 2;

/**
 * Maximum length of a WiFi SSID.
 */
constexpr size_t WifiMaximumSsidLength = 64;

/**
 * Maximum length of a WiFi password.
 */
constexpr size_t WifiMaximumPasswordLength = 64;

/**
 * How long the user should hold the button to initiate a factory wipe.
 */
constexpr uint32_t InteractiveStartupButtonDuration = TwoSecondsMs;

/**
 * Size of a page in EEPROM memory.
 */
constexpr size_t CodeMemoryPageSize = 512;

/**
 * Size of a block in EEPROM memory.
 */
constexpr size_t CodeMemoryBlockSize = 8192;

/**
 * This is the theorhetical maximum number of modules that can be physically
 * connected to a single station. This is governed by the largest backplanes and
 * their possible combinations.
 */
constexpr size_t MaximumNumberOfPhysicalModules = 4;

/**
 * The number of fixed HTTP routes that the firmware registers. These are routes
 * that are always there and never change with configuration.
 */
constexpr size_t HttpFixedRoutes = 4;

/**
 * The maximum number of HTTP routes that can be registered. Trying to register
 * more than this will fail. This is a function of the maximum number of modules
 * and the number of fixed routes.
 */
constexpr size_t HttpMaximumRoutes = MaximumNumberOfPhysicalModules + HttpFixedRoutes;

/**
 * The maximum length of an incoming URL.
 */
constexpr size_t HttpdMaximumUrlLength = 64;

/**
 * How long to wait for a WiFi connection to establish.
 */
constexpr uint32_t NetworkConnectionTimeoutMs = 30 * 1000;

constexpr uint32_t NetworkConnectionMaximumDuration = FiveSecondsMs;

/**
 * Buffer size for storing GPS sentences for debugging purposes.
 */
constexpr size_t GpsDebuggingBuffer = 64;

/**
 * Maximum size of all the headers in an HTTP request.
 */
constexpr size_t HttpMaximumHeaderSize = 1024;

/**
 *
 */
constexpr size_t HttpConnectionBufferSize = 1024;

/**
 *
 */
constexpr size_t HttpConnectionWorkSize = DefaultWorkerPoolSize;

/**
 * Maximum number of blocks to look ahead for an available block. This means we
 * can't deal with this many bad blocks sequentially.
 */
constexpr int32_t StorageAvailableBlockLookAhead = 10;

/**
 * How often to display progress information.
 */
constexpr uint32_t ProgressIntervalMs = 1000;

/**
 * Amount of pooled memory to allocate for taking readings and doing module
 * related work.
 */
constexpr size_t ModuleMemoryAreaSize = 8192;

/**
 * Maximum number of memory banks we're capable of supporting.
 */
#define FK_MAXIMUM_NUMBER_OF_MEMORY_BANKS (2)

/**
 * Maximum number of memory banks we're capable of supporting.
 */
constexpr size_t MaximumNumberOfMemoryBanks = FK_MAXIMUM_NUMBER_OF_MEMORY_BANKS;

/**
 * Size of the bootloader.
 */
constexpr uint32_t BootloaderSize = 0x8000;

/**
 * Address of the secondary bank of flash memory.
 */
constexpr uint32_t OtherBankAddress = 0x80000;

/**
 * Number of worker tasks to allow.
 */
constexpr size_t NumberOfWorkerTasks = 2;

/**
 * True to enable dumping memory, hash comparisons, etc...
 */
constexpr bool EnableMemoryDumps = false;

/**
 * Runtime configuration informationthat is unavailable in the mobile
 * application. This can be updated after compilation to build binaries for
 * various situations.
 */
typedef struct configuration_t {
    /**
     * Run a full self check, even for things that don't necessarily need to be
     * working. This affects checking GPS and SD Card as well as backplane pieces.
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
     * Network related configuration.
     */
    typedef struct network_t {
        /**
         * How long network is kept up and available before turning off due to inactivity.
         */
        uint32_t uptime{ FiveMinutesMs };
    } network_t;

    /**
     * Network configuration.
     */
    network_t network;

    /**
     * Configuration related to collecting readings.
     */
    typedef struct readings_t {
        /**
         * If true then the diagnostics module is enabled and will be consulted
         * for readings like battery and memory information.
         */
        bool enable_diagnostics_module{ true };

        /**
         * If true then the random module is enabled and will be consulted for
         * readings, this can inflate the data being written, which is handy for
         * debugging.
         */
        bool enable_random_module{
            #if defined(FK_ENABLE_RANDOM)
            true
            #else
            false
            #endif
        };

        /**
         * Amplification factor.
         */
        size_t amplification{ 1 };
    } readings_t;

    /**
     * Readings collection configuration.
     */
    readings_t readings;

    typedef struct display_t {
        uint32_t inactivity{ FiveMinutesMs };
    } display_t;

    /**
     * Display configuration.
     */
    display_t display;

    typedef struct scheduler_t {
        uint32_t readings_interval{ 60 };
        uint32_t network_interval{ 300 * 4 };
        uint32_t lora_interval{ 300 };
        uint32_t gps_interval{ OneDaySeconds };

        /**
         * Number of ms to try for a fix.
         */
        uint32_t fix_waiting{ FiveMinutesMs };

        /**
         * Number of ms to wait with the fix.
         */
        uint32_t fix_hold{ OneMinuteMs };
    } scheduler_t;

    /**
     * Scheduler configuration.
     */
    scheduler_t scheduler;

    typedef struct lora_t {
        /**
         * Transmit virtual readings over LoRa.
         */
        bool transmit_virtual{ false };
    } lora_t;

    /**
     * LoRa configuration.
     */
    lora_t lora;

    bool modules_always_on{ false };
} configuration_t;

configuration_t const &fk_config();

}
