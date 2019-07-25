#include "config.h"
#include "secrets.h"

namespace fk {

configuration_t fkc;

configuration_t &fk_config_initialize() {
    #if defined(FK_WIFI_0_SSID) && defined(FK_WIFI_0_PASSWORD)
    fkc.network.networks[0].ssid = FK_WIFI_0_SSID;
    fkc.network.networks[0].password = FK_WIFI_0_PASSWORD;
    #endif
    #if defined(FK_WIFI_1_SSID) && defined(FK_WIFI_1_PASSWORD)
    fkc.network.networks[1].ssid = FK_WIFI_1_SSID;
    fkc.network.networks[1].password = FK_WIFI_1_PASSWORD;
    #endif
    return fkc;
}

configuration_t &fk_config() {
    return fkc;
}

}
