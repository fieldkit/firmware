#include "fk.h"
#include "display.h"
#include "u8g2_display.h"
#include "logos/conservify_bw_logo-56.h"
#include "logos/fk_bw-128x27.h"

namespace fk {

/**
 * I2C address for the SH1106 driver for the OLED display. Note that the u8g2
 * library relies on this being the read I2C address.
 */
constexpr uint8_t OLED_ADDRESS = 0x3C * 2;

/**
 * Width of the OLED display in pixels. Adjusted for the display's physical alignment.
 */
constexpr uint8_t OLED_WIDTH = 128;

/**
 * Height of the OLED display in pixels. Adjusted for the display's physical alignment.
 */
constexpr uint8_t OLED_HEIGHT = 56;

/**
 * Offset on the Y axis.
 */
constexpr uint8_t OLED_X_OFFSET = 0;

/**
 * Offset on the Y axis.
 */
constexpr uint8_t OLED_Y_OFFSET = 8;

xbm_data_t conservify_bw_logo_56 = {
    .w = CONSERVIFY_BW_LOGO_56_WIDTH,
    .h = CONSERVIFY_BW_LOGO_56_HEIGHT,
    .data = conservify_bw_logo_56_data,
};

xbm_data_t fk_bw_logo_128x27 = {
    .w = FK_BW_128X27_WIDTH,
    .h = FK_BW_128X27_HEIGHT,
    .data = fk_bw_128x27_data,
};

U8g2Display::U8g2Display() : draw_(U8G2_R0, U8X8_PIN_NONE, PIN_WIRE_SCL, PIN_WIRE_SDA) {
    draw_.setI2CAddress(OLED_ADDRESS);
}

bool U8g2Display::begin() {
    if (!draw_.begin()) {
        available_ = false;

        return false;
    }

    available_ = true;

    off();

    return true;
}

void U8g2Display::on() {
    draw_.clearBuffer();
    draw_.setPowerSave(0);
    draw_.sendBuffer();
}

void U8g2Display::centered(xbm_data_t &xbm) {
    auto x = (OLED_WIDTH / 2) - (xbm.w / 2) + OLED_X_OFFSET;
    auto y = (OLED_HEIGHT / 2) - (xbm.h / 2) + OLED_Y_OFFSET;
    draw_.drawXBM(x, y, xbm.w, xbm.h, xbm.data);
}

void U8g2Display::company_logo() {
    draw_.setPowerSave(0);
    draw_.clearBuffer();
    centered(conservify_bw_logo_56);
    draw_.sendBuffer();
}

void U8g2Display::fk_logo() {
    draw_.setPowerSave(0);
    draw_.clearBuffer();
    centered(fk_bw_logo_128x27);
    draw_.sendBuffer();
}

void U8g2Display::off() {
    draw_.setPowerSave(1);
}

static NullDisplay null_display;
static U8g2Display u8g2_display;

Display *DisplayFactory::get_display() {
    if (u8g2_display.begin()) {
        return &u8g2_display;
    }
    return &null_display;
}

}
