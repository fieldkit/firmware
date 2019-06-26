#include "fk.h"
#include "u8g2_display.h"
#include "printf.h"

#if defined(FK_HARDWARE_FULL)

namespace fk {

#include "logos/conservify_logo_bw-56.h"
#include "logos/fk_logo_bw-128x27.h"
#include "logos/fk_logo_bw-100x21.h"
#include "logos/fk_logo_bw-80x17.h"

constexpr char glyph_open_iconic_all_arrow_door = '\x40';
constexpr char glyph_open_iconic_all_clock = '\x7B';
constexpr char glyph_open_iconic_all_gps = '\xD1';
constexpr char glyph_open_iconic_all_wifi_1 = '\xF7';
constexpr char glyph_open_iconic_all_wifi_2 = '\xF8';
constexpr char glyph_open_iconic_all_bug = '\x68';
constexpr char glyph_open_iconic_all_glass = '\xCF';

constexpr char glyph_battery19[] = { '1', '2', '3', '4', '5' };

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

void U8g2Display::centered(const xbm_data_t &xbm) {
    auto x = (OLED_WIDTH / 2) - (xbm.w / 2) + OLED_X_OFFSET;
    auto y = (OLED_HEIGHT / 2) - (xbm.h / 2) + OLED_Y_OFFSET;
    draw_.drawXBM(x, y, xbm.w, xbm.h, xbm.data);
}

void U8g2Display::company_logo() {
    draw_.setPowerSave(0);
    draw_.clearBuffer();
    centered(conservify_logo_bw_56);
    draw_.sendBuffer();
}

void U8g2Display::fk_logo() {
    draw_.setPowerSave(0);
    draw_.clearBuffer();
    centered(fk_logo_bw_128x27);
    draw_.sendBuffer();
}

static bool toggle_every(uint32_t time, uint32_t interval) {
    return (time / interval ) % 2 == 0;
}

static int32_t lerp(int32_t start, int32_t end, uint32_t interval, uint32_t time) {
    auto scale = ((float)(time % interval) / (float)interval);
    return start + (int32_t)((float)(end - start) * scale);
}

void U8g2Display::home(home_screen_t data) {
    draw_.setPowerSave(0);
    draw_.clearBuffer();

    auto &logo = fk_logo_bw_80x17;
    draw_.drawXBM(2, logo.h - 4, logo.w, logo.h, logo.data);

    draw_.setFont(u8g2_font_battery19_tn);
    draw_.drawGlyph(OLED_WIDTH - 8, 30, glyph_battery19[sizeof(glyph_battery19) - 1]);

    draw_.setFont(u8g2_font_open_iconic_all_2x_t);
    if (toggle_every(data.time, 1000)) {
        draw_.drawGlyph(OLED_WIDTH - 44, 30, glyph_open_iconic_all_wifi_1);
    }
    else {
        draw_.drawGlyph(OLED_WIDTH - 44, 30, glyph_open_iconic_all_wifi_2);
    }

    draw_.setFont(u8g2_font_open_iconic_all_2x_t);
    if (toggle_every(data.time, 1000)) {
        draw_.drawGlyph(OLED_WIDTH - 28, 30, glyph_open_iconic_all_gps);
    }
    else {
        draw_.drawGlyph(OLED_WIDTH - 28, 30, glyph_open_iconic_all_gps);
    }

    char buffer[128];
    fk_snprintf(buffer, sizeof(buffer), "Temp: 22.4°C pH: 8.32");
    draw_.setFontMode(0);
    draw_.setFont(u8g2_font_courB18_tf);
    auto width = draw_.getUTF8Width(buffer);
    auto x = lerp(-width, OLED_WIDTH, 10000, data.time);
    draw_.drawUTF8(x, OLED_HEIGHT, buffer);

    draw_.sendBuffer();
}

void U8g2Display::off() {
    draw_.setPowerSave(1);
}

}

#endif
