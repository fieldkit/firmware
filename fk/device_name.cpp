#include <tiny_printf.h>
#include <phylum.h>

#include "device_name.h"
#include "platform.h"
#include "hal/random.h"

namespace fk {

#include "data/adjectives.h"
#include "data/animals.h"

const char *fk_device_name_generate(Pool &pool) {
    auto animal_index = fk_random_i32(0, animals_size);
    auto adjective_index = fk_random_i32(0, adjectives_size);
    auto animal = animals[animal_index];
    auto adjective = adjectives[adjective_index];
    auto suffix = fk_random_i32(0, 100);

    return pool.sprintf("%s %s %d", adjective, animal, suffix);
}

}
