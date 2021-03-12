#pragma once

#ifdef __cplusplus
extern "C" {
#endif

int32_t fk_wdt_initialize();

int32_t fk_wdt_enable();

int32_t fk_wdt_disable();

int32_t fk_wdt_feed();

namespace fk {

class EnableWatchdog {
public:
    EnableWatchdog();
    virtual ~EnableWatchdog();

};

}

#ifdef __cplusplus
}
#endif
