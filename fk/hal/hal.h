#pragma once

/**
 * RADIO_MOSI  PB26
 * RADIO_MISO  PB29
 * RADIO_SCK   PB27
 *
 * WIFI_CS     PB05
 * WIFI_ENABLE PC25
 * WIFI_RESET  PC26
 * WIFI_IRQ    PC27
 *
 * WIFI_POWER  PB07
 *
 * GPS_TX      PB24
 * GPS_RX      PB25
 * GPS_POWER   PB06
 *
 * Wire1
 * RAD_SDA     PC22
 * RAD_SCL     PC23
 */

#include "common.h"
#include "platform.h"
#include "wifi.h"
#include "metal.h"

namespace fk {

constexpr uint8_t GPS_POWER = 55u;

}
