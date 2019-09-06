#include "weather_module.h"
#include "utilities.h"
#include "modules/shared/crc.h"
#include "modules/eeprom.h"
#include "weather.h"

using namespace fk;

FK_DECLARE_LOGGER("weather");

bool WeatherModule::initialize(ModuleContext mc, fk::Pool &pool) {
    return true;
}

static SensorMetadata const fk_module_weather_sensor_metas[] = {
    { .name = "temperature 1", .unitOfMeasure = "C",   .flags = 0 },
    { .name = "temperature 2", .unitOfMeasure = "C",   .flags = 0 },
    { .name = "pressure",      .unitOfMeasure = "kPa", .flags = 0 },
    { .name = "humidity",      .unitOfMeasure = "%",   .flags = 0 },
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

ModuleReadings *WeatherModule::take_readings(ModuleContext mc, fk::Pool &pool) {
    auto module_bus = get_board()->i2c_module();
    ModuleEeprom eeprom{ module_bus };

    fk_weather_t reading;
    uint32_t found_address = 0;
    bzero(&reading, sizeof(fk_weather_t));

    for (uint32_t address = EEPROM_ADDRESS_READINGS; address < EEPROM_ADDRESS_READINGS_END; address += sizeof(fk_weather_t)) {
        fk_weather_t temp;

        if (!eeprom.read_data(address, &temp, sizeof(fk_weather_t))) {
            logerror("error reading eeprom");
            return nullptr;
        }

        auto expected = calculate_crc(FK_MODULES_CRC_SEED, temp);
        if (expected == temp.crc) {
            if (found_address == 0 || temp.seconds > reading.seconds) {
                reading = temp;
            }
            found_address = address;
        }
        else {
            loginfo("[0x%04" PRIx32 "] bad crc (%" PRIx32 " != %" PRIx32 ")", address, expected, temp.crc);
            fk_dump_memory("WTR ", (uint8_t *)&temp, sizeof(temp));
            break;
        }
    }

    if (found_address == 0) {
        logerror("no readings");
        return nullptr;
    }

    loginfo("found reading: %" PRIu32 " 0x04%" PRIx32, reading.seconds, found_address);
    loginfo("humidity: %" PRIu32, reading.humidity);
    loginfo("temperature-1: %" PRIu32, reading.temperature_1);
    loginfo("pressure: %" PRIu32, reading.pressure);
    loginfo("temperature-2: %" PRIu32, reading.temperature_2);

    auto i = 0;
    auto mr = new(pool) NModuleReadings<4>();
    mr->set(i++, reading.humidity);
    mr->set(i++, reading.temperature_1);
    mr->set(i++, reading.pressure);
    mr->set(i++, reading.temperature_2);
    return mr;
}
