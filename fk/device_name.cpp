#include <tiny_printf.h>
#include <phylum/crc.h>

#include "device_name.h"
#include "platform.h"
#include "hal/random.h"

namespace fk {

#include "data/adjectives.h"
#include "data/animals.h"

const char *fk_device_name_generate(Pool &pool) {
    fk_serial_number_t sn;
    fk_serial_number_get(&sn);

    auto key = phylum::crc32_checksum((uint8_t *)&sn, sizeof(fk_serial_number_t));
    auto animal_index = key % animals_size;
    auto adjective_index = key % adjectives_size;
    auto animal = animals[animal_index];
    auto adjective = adjectives[adjective_index];
    auto suffix = key % 100;

    return pool.sprintf("%s %s %d", adjective, animal, suffix);
}

const char *fk_device_name_printf(char *str, size_t size) {
    fk_serial_number_t sn;
    fk_serial_number_get(&sn);

    auto key = phylum::crc32_checksum((uint8_t *)&sn, sizeof(fk_serial_number_t));
    auto animal_index = key % animals_size;
    auto adjective_index = key % adjectives_size;
    auto animal = animals[animal_index];
    auto adjective = adjectives[adjective_index];
    auto suffix = key % 100;

    auto needed = tiny_snprintf(str, size, "%s %s %d", adjective, animal, suffix);
    if ((size_t)needed >= size) {
        return nullptr;
    }

    return str;
}

}
