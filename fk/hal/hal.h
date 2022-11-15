#pragma once

#include "common.h"
#include "platform.h"

#include "hal/board.h"

#include "hal/clock.h"
#include "hal/watchdog.h"
#include "hal/random.h"
#include "hal/network.h"
#include "hal/memory.h"
#include "hal/gps.h"
#include "hal/display.h"
#include "hal/battery_gauge.h"
#include "hal/buttons.h"
#include "hal/ipc.h"
#include "hal/modmux.h"
#include "hal/leds.h"
#include "hal/marker_lights.h"

#if defined(__SAMD51__)
#include "hal/metal/metal.h"
#else
#include "hal/linux/linux.h"
#endif
