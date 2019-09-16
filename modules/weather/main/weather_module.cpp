#include "weather_module.h"
#include "utilities.h"
#include "modules/shared/crc.h"
#include "modules/eeprom.h"
#include "weather.h"

using namespace fk;

FK_DECLARE_LOGGER("weather");

static SensorMetadata const fk_module_weather_sensor_metas[] = {
    { .name = "humidity",      .unitOfMeasure = "%",      .flags = 0 },
    { .name = "temperature 1", .unitOfMeasure = "C",      .flags = 0 },
    { .name = "pressure",      .unitOfMeasure = "kPa",    .flags = 0 },
    { .name = "temperature 2", .unitOfMeasure = "C",      .flags = 0 },
    { .name = "wind",          .unitOfMeasure = "kmh",    .flags = 0 },
    { .name = "rain",          .unitOfMeasure = "inches", .flags = 0 },
};

static ModuleSensors fk_module_weather_sensors = {
    .nsensors = sizeof(fk_module_weather_sensor_metas) / sizeof(SensorMetadata),
    .sensors = fk_module_weather_sensor_metas,
};

ModuleSensors const *WeatherModule::get_sensors(ModuleContext mc, Pool &pool) {
    return &fk_module_weather_sensors;
}

template<typename T>
static uint32_t calculate_crc(uint32_t seed, T &object) {
    return crc32_checksum(seed, (uint8_t *)&object, sizeof(T) - sizeof(uint32_t));
}

bool WeatherModule::initialize(ModuleContext mc, fk::Pool &pool) {
    auto module_bus = get_board()->i2c_module();
    auto eeprom = ModuleEeprom{ module_bus };

    fk_weather_t reading;
    bzero(&reading, sizeof(fk_weather_t));
    address_ = 0;
    seconds_ = 0;

    // Scan all of the EEPROM to find the MAX(seconds) reading. This will be the
    // end of them and we'll be resuming from there.

    uint32_t iter = EEPROM_ADDRESS_READINGS;
    while (true) {
        if (iter + sizeof(fk_weather_t) >= EEPROM_ADDRESS_READINGS_END) {
            break;
        }

        fk_weather_t temp;
        if (!eeprom.read_data(iter, &temp, sizeof(fk_weather_t))) {
            logerror("error reading eeprom");
            return nullptr;
        }

        auto expected = calculate_crc(FK_MODULES_CRC_SEED, temp);
        if (expected == temp.crc) {
            logdebug("[0x%04" PRIx32 "] reading (%" PRIu32 ")", iter, temp.seconds);
            if (address_ == 0 || temp.seconds > reading.seconds) {
                reading = temp;
                seconds_ = reading.seconds;
                address_ = iter;
            }
        }
        else {
            loginfo("[0x%04" PRIx32 "] bad crc (0x%" PRIx32 " != 0x%" PRIx32 ")", iter, expected, temp.crc);
            break;
        }

        iter += sizeof(fk_weather_t);
    }

    if (address_ > 0) {
        loginfo("[0x%04" PRIx32 "] end (%" PRIu32 ")", address_, seconds_);
    }

    return true;
}

ModuleReadings *WeatherModule::take_readings(ModuleContext mc, fk::Pool &pool) {
    static constexpr float RainPerTick = 0.2794; // mm
    static constexpr float WindPerTick = 2.4; // km/hr

    auto module_bus = get_board()->i2c_module();
    auto eeprom = ModuleEeprom{ module_bus };

    // Keep trying to find the end if we weren't able to.
    if (address_ == 0) {
        if (!initialize(mc, pool)) {
            logerror("initialize failed");
            return nullptr;
        }

        if (address_ == 0) {
            logerror("no readings");
            return nullptr;
        }
    }

    fk_weather_t reading;
    bzero(&reading, sizeof(fk_weather_t));

    uint32_t rain_ticks = 0;
    uint32_t wind_ticks = 0;

    while (true) {
        if (address_ + sizeof(fk_weather_t) >= EEPROM_ADDRESS_READINGS_END) {
            address_ = EEPROM_ADDRESS_READINGS;
        }

        fk_weather_t temp;
        if (!eeprom.read_data(address_, &temp, sizeof(fk_weather_t))) {
            logerror("error reading eeprom");
            return nullptr;
        }

        auto expected = calculate_crc(FK_MODULES_CRC_SEED, temp);
        if (expected == temp.crc) {
            if (temp.seconds < seconds_) {
                break;
            }
            logdebug("[0x%04" PRIx32 "] reading (%" PRIu32 ")", address_, seconds_);
            reading = temp;
            seconds_ = temp.seconds;
            rain_ticks += reading.rain.ticks;
            wind_ticks += reading.wind.ticks;
        }
        else {
            loginfo("[0x%04" PRIx32 "] bad crc (0x%" PRIx32 " != 0x%" PRIx32 ")", address_, expected, temp.crc);
            break;
        }

        address_ += sizeof(fk_weather_t);
    }

    if (reading.seconds == 0) {
        logwarn("no readings");
        return nullptr;
    }

    auto i = 0;
    auto mr = new(pool) NModuleReadings<6>();
    mr->set(i++, 100.0f * ((float)reading.humidity / (0xffff)));
    mr->set(i++, -45.0f + 175.0f * ((float)reading.temperature_1 / (0xffff)));
    mr->set(i++, reading.pressure / 64.0f / 1000.0f);
    mr->set(i++, reading.temperature_2 / 16.0f);
    mr->set(i++, rain_ticks * RainPerTick);
    mr->set(i++, wind_ticks * WindPerTick);
    return mr;
}
