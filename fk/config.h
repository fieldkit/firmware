#pragma once

#include <inttypes.h>
#include <string.h>

namespace fk {

// -------------------------------------------------------------------------------------------
// Time Intervals

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
 * One minute in seconds.
 */
constexpr uint32_t OneMinuteSeconds = 60;

/**
 * Five minutes in seconds.
 */
constexpr uint32_t FiveMinutesSeconds = 5 * 60;

/**
 * Five minutes in milliseconds.
 */
constexpr uint32_t FiveMinutesMs = FiveMinutesSeconds * 1000;

/**
 * Ten minutes in seconds.
 */
constexpr uint32_t TenMinutesSeconds = FiveMinutesSeconds * 2;

/**
 * Ten minutes in milliseconds.
 */
constexpr uint32_t TenMinutesMs = FiveMinutesSeconds * 1000 * 2;

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
 * Threshold for clock drift warning.
 */
constexpr int32_t TimeDriftWarnThreshold = 5;

// -------------------------------------------------------------------------------------------
// Sizes

/**
 * Precisely 1024 * 1024.
 */
constexpr uint32_t OneMegabyte = 1024 * 1024;

// -------------------------------------------------------------------------------------------
// Schedules and Time

constexpr uint32_t DefaultReadingsInterval = 60;
constexpr uint32_t DefaultNetworkInterval = 60 * 60 * 60;
constexpr uint32_t DefaultLoraInterval = 60 * 60 * 2;
constexpr uint32_t DefaultGpsInterval = OneDaySeconds;
constexpr uint32_t DefaultSynchronizeTimeInterval = OneDaySeconds;
constexpr uint32_t DefaultDebugReadingsInterval = 60;
constexpr uint32_t DefaultDebugNetworkInterval = 60 * 60 * 60;
constexpr uint32_t DefaultDebugLoraInterval = 60 * 3;
constexpr uint32_t DefaultDebugGpsInterval = OneDaySeconds;

/**
 * Maximum allowed time drift, in seconds.
 */
constexpr uint32_t AcceptableTimeDriftSeconds = 5;

/**
 * How long the user should hold the button to initiate a factory wipe.
 */
constexpr uint32_t InteractiveStartupButtonDuration = TwoSecondsMs;

/**
 * How often to display progress information.
 */
constexpr uint32_t ProgressIntervalMs = 1000;

/**
 * How often to check for configuration changes.
 */
constexpr uint32_t ConfigurationCheckIntervalMs = 500;

// -------------------------------------------------------------------------------------------
// Memory

/**
 * Standard page size to allocate for operations.
 */
constexpr size_t StandardPageSize = 8192;

/**
 * Size of the circular buffer that stores logs.
 */
constexpr size_t InMemoryLogBufferSize = 32768;

/**
 * Size of the network buffers.
 */
constexpr size_t NetworkBufferSize = 1024;

/**
 * Maximum size to use for buffers allocated on the stack.
 */
constexpr size_t StackBufferSize = 128;

/**
 * Size of a page in EEPROM memory.
 */
constexpr size_t CodeMemoryPageSize = 512;

/**
 * Size of a block in EEPROM memory.
 */
constexpr size_t CodeMemoryBlockSize = 8192;

// -------------------------------------------------------------------------------------------
// Modules

/**
 * If true then modules will never be turned off to optimize power consumption.
 */
constexpr bool ModulesAlwaysOn = false;

/**
 * If enabled the virtual diagnostics module will be scanned.
 */
constexpr bool ModulesEnableDiagnostics = true;

/**
 * If enabled the virtual random module will be scanned.
 */
constexpr bool ModulesEnableRandom = false;

/**
 * This is the theorhetical maximum number of modules that can be physically
 * connected to a single station. This is governed by the largest backplanes and
 * their possible combinations.
 */
constexpr size_t MaximumNumberOfPhysicalModules = 4;

// -------------------------------------------------------------------------------------------
// LoRa

/**
 * How often to save LoRa radio state.
 */
constexpr uint32_t LoraUplinksSaveFrequency = 10;

/**
 * If enabled then the device will transmit readings from virtual modules.
 */
constexpr bool LoraTransmitVirtual = false;

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
 * Number of times to try a LoRa transmission.
 */
constexpr size_t LoraSendTries = 3;

/**
 * Delay between LoRa packets.
 */
constexpr uint32_t LoraPacketDelay = TenSecondsMs;

/**
 * LoRa activation by personalization settings.
 */
struct LoraAbpSettings {
    uint8_t device_eui[LoraDeviceEuiLength];
    uint8_t device_address[LoraDeviceAddressLength];
    uint8_t network_session_key[LoraNetworkSessionKeyLength];
    uint8_t app_session_key[LoraAppSessionKeyLength];
};

// -------------------------------------------------------------------------------------------
// Field Lengths

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

// -------------------------------------------------------------------------------------------
// Network/Wifi

/**
 * How long to wait for a WiFi connection to establish.
 */
constexpr uint32_t NetworkConnectionTimeoutMs = 30 * 1000;

/**
 * Maximum duration to allow an inactive connection open. This is
 * adjusted to be higher than the status message frequency so that
 * debug connections aren't killed prematurely.
 */
constexpr uint32_t NetworkConnectionMaximumDuration = TenSecondsMs;

/**
 * Maximum number of WiFi networks.
 */
constexpr size_t WifiMaximumNumberOfNetworks = 5;

/**
 * Maximum length of a WiFi SSID.
 */
constexpr size_t WifiMaximumSsidLength = 64;

/**
 * Maximum length of a WiFi password.
 */
constexpr size_t WifiMaximumPasswordLength = 64;

/**
 * The number of fixed HTTP routes that the firmware registers. These are routes
 * that are always there and never change with configuration.
 */
constexpr size_t HttpFixedRoutes = 5;

/**
 * The maximum number of HTTP routes that can be registered. Trying to register
 * more than this will fail. This is a function of the maximum number of modules
 * and the number of fixed routes.
 */
constexpr size_t HttpMaximumRoutes = MaximumNumberOfPhysicalModules + HttpFixedRoutes;

/**
 * Maximum size of all the headers in an HTTP request.
 */
constexpr size_t HttpMaximumHeaderSize = 1024;

/**
 * Buffer size to use for HTTP operations.
 */
constexpr size_t HttpConnectionBufferSize = 1024;

/**
 * Maximum length of API urls.
 */
constexpr size_t HttpMaximumUrlLength = 64;

/**
 * Maximum length of API authentication tokens.
 */
constexpr size_t HttpMaximumTokenLength = 512;

// -------------------------------------------------------------------------------------------
// GPS

/**
 * Buffer size for storing GPS sentences for debugging purposes.
 */
constexpr size_t GpsDebuggingBuffer = 64;

/**
 * Flag to enable logging of raw GPS data.
 */
constexpr bool GpsLoggingRaw = false;

// -------------------------------------------------------------------------------------------
// Storage

/**
 * Maximum number of memory banks we're capable of supporting.
 */
// #define FK_MAXIMUM_NUMBER_OF_MEMORY_BANKS (2)

/**
 * Maximum number of blocks to look ahead for an available block. This means we
 * can't deal with this many bad blocks sequentially.
 */
constexpr int32_t StorageAvailableBlockLookAhead = 10;

/**
 * Maximum number of memory banks we're capable of supporting.
 */
constexpr size_t StorageMaximumNumberOfMemoryBanks = 2;

// -------------------------------------------------------------------------------------------
// Debug

/**
 * True to enable dumping memory, hash comparisons, etc...
 */
constexpr bool DebugEnableMemoryDumps = false;

// -------------------------------------------------------------------------------------------
// Basically Fixed

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

}
