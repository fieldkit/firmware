#include <Base64.h>

#include "device_name.h"
#include "platform.h"
#include "hal/random.h"

namespace fk {

#include "data/adjectives.h"
#include "data/animals.h"

const char *fk_device_name_generate(Pool &pool) {
    fk_serial_number_t sn;
    fk_serial_number_get(&sn);

    // TODO This should be better but will get things up and running.
    // Consider moving to:
    // https://en.wikipedia.org/wiki/Linear-feedback_shift_register
    auto key = sn.dwords[0] + sn.dwords[1] + sn.dwords[2] + sn.dwords[3];
    auto animal_index = key % animals_size;
    auto adjective_index = key % adjectives_size;
    auto animal = animals[animal_index];
    auto adjective = adjectives[adjective_index];
    auto suffix = key % 100;

    return pool.sprintf("%s %s %d", adjective, animal, suffix);
}

}