#include "config.h"

namespace fk {

configuration_t fkc;

configuration_t const &fk_config_initialize() {
    return fkc;
}

configuration_t const &fk_config() {
    return fkc;
}

}
