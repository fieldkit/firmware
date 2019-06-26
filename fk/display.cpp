#include "fk.h"
#include "display.h"
#include "u8g2_display.h"

namespace fk {

static NullDisplay null_display;
#if defined(FK_HARDWARE_FULL)
static U8g2Display u8g2_display;
#else
static NullDisplay u8g2_display;
#endif

Display *DisplayFactory::get_display() {
    if (u8g2_display.begin()) {
        return &u8g2_display;
    }
    return &null_display;
}

}
