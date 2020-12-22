#include "fk.h"

#include "hal/display.h"
#include "hal/metal/u8g2_display.h"

namespace fk {

MenuScreen::MenuScreen() : options(nullptr) {
}

MenuScreen::MenuScreen(const char *title, MenuOption **options) : title(title), options(options) {
    options[0]->focused_ = true;
}

static NullDisplay null_display;
#if defined(FK_HARDWARE_FULL)
static U8g2Display u8g2_display;
#else
static NullDisplay u8g2_display;
#endif

static Display *picked = nullptr;

Display *get_display() {
    if (picked == nullptr) {
        if (u8g2_display.begin()) {
            picked = &u8g2_display;
        }
        else {
            picked = &null_display;
        }
        FK_ASSERT(picked != nullptr);
    }
    return picked;
}

}
