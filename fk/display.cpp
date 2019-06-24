#include "fk.h"
#include "display.h"
#include "u8g2_display.h"

namespace fk {

static NullDisplay null_display;
static U8g2Display u8g2_display;

Display *DisplayFactory::get_display() {
    if (u8g2_display.begin()) {
        return &u8g2_display;
    }
    return &null_display;
}

}
