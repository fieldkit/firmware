#pragma once

namespace fk {

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
} configuration_t;

/**
 * Singleton instance of the runtime configuration data.
 */
extern configuration_t fkc;

}
