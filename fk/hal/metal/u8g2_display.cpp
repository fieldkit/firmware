#include <tiny_printf.h>

#include "fk.h"
#include "hal/metal/u8g2_display.h"

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

constexpr char glyph_open_cursor_all_recording = '\x47';

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
constexpr uint8_t OLED_HEIGHT = 64;

/**
 * Offset on the Y axis.
 */
constexpr uint8_t OLED_X_OFFSET = 0;

/**
 * Offset on the Y axis.
 */
constexpr uint8_t OLED_Y_OFFSET = 0;

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

/*
static int32_t lerp(int32_t start, int32_t end, uint32_t interval, uint32_t time) {
    auto scale = ((float)(time % interval) / (float)interval);
    return start + (int32_t)((float)(end - start) * scale);
}
*/

template<typename T>
static void draw_centered(T draw, uint16_t y, const char *str) {
    auto width = draw.getUTF8Width(str);
    draw.drawUTF8((OLED_WIDTH / 2) - (width / 2), y, str);
}

template<typename T>
static bool draw_string_auto_sized(T draw, bool bold, uint16_t x, uint16_t y, uint16_t w, uint16_t h, const char *str) {
    constexpr size_t NumberFaces = 5;

    uint8_t const *bold_faces[NumberFaces] = {
        u8g2_font_courB14_tf,
        u8g2_font_courB14_tf,
        u8g2_font_courB12_tf,
        u8g2_font_courB10_tf,
        u8g2_font_courB08_tf,
    };

    uint8_t const *normal_faces[NumberFaces] = {
        u8g2_font_courR14_tf,
        u8g2_font_courR14_tf,
        u8g2_font_courR12_tf,
        u8g2_font_courR10_tf,
        u8g2_font_courR08_tf,
    };

    auto faces = bold ? bold_faces : normal_faces;

    for (auto i = 0u; i < NumberFaces; ++i) {
        draw.setFontMode(0);
        draw.setFont(faces[i]);
        auto width = draw.getUTF8Width(str);
        if (width <= w) {
            auto fx = x + ((w / 2) - (width / 2));
            auto fy = y + h;
            draw.drawUTF8(fx, fy, str);
            return true;
        }
    }

    return false;
}

static void pretty_bytes(uint32_t bytes, uint32_t *value, const char **suffix) {
    if (bytes > 1024 * 1024) {
        *value = bytes / (1024 * 1024);
        *suffix = "MB";
    }
    else if (bytes > 1024) {
        *value = bytes / (1024);
        *suffix = "kB";
    }
    else {
        *value = bytes;
        *suffix = "B";
    }
}

void U8g2Display::home(HomeScreen const &data) {
    draw_.setPowerSave(0);
    draw_.clearBuffer();

    if (data.progress.operation == nullptr) {
        char buffer[64];
        if (data.network.enabled) {
            if (toggle_every(data.time, 5000)) {
                const char *rx_suffix = "";
                uint32_t rx_pretty = 0;

                pretty_bytes(data.network.bytes_tx, &rx_pretty, &rx_suffix);

                tiny_snprintf(buffer, sizeof(buffer), "%d%s tx", rx_pretty, rx_suffix);
                draw_.setFontMode(0);
                draw_.setFont(u8g2_font_courB08_tf);
                auto width = draw_.getUTF8Width(buffer);
                draw_.drawUTF8(((OLED_WIDTH - 48) / 2) - (width / 2), 12, buffer);
            }
            else {
                tiny_snprintf(buffer, sizeof(buffer), "%d", data.readings);
                draw_.setFontMode(0);
                draw_.setFont(u8g2_font_courB08_tf);
                auto width = draw_.getUTF8Width(buffer);
                draw_.drawUTF8(((OLED_WIDTH - 48) / 2) - (width / 2), 12, buffer);
            }
        }
        else {
            if (data.debug_mode) {
                const char *text = "Debug Mode";
                auto width = draw_.getUTF8Width(text);
                draw_.setFontMode(0);
                draw_.setFont(u8g2_font_courB08_tf);
                draw_.drawUTF8(((OLED_WIDTH - 48) / 2) - (width / 2), 12, text);
            }
            else {
                tiny_snprintf(buffer, sizeof(buffer), "%d", data.readings);
                draw_.setFontMode(0);
                draw_.setFont(u8g2_font_courB08_tf);
                auto width = draw_.getUTF8Width(buffer);
                draw_.drawUTF8(((OLED_WIDTH - 48) / 2) - (width / 2), 12, buffer);
            }
        }
    }
    else {
        // TODO Modify this so that the percentage is full size and the label varies.
        char buffer[128];
        tiny_snprintf(buffer, sizeof(buffer), "%s %.1f", data.progress.operation, data.progress.progress);
        draw_.setFontMode(0);
        draw_.setFont(u8g2_font_courB08_tf);
        auto width = draw_.getUTF8Width(buffer);
        draw_.drawUTF8(((OLED_WIDTH - 44) / 2) - (width / 2), 12, buffer);
    }

    if (data.recording) {
        if (toggle_every(data.time, 1000)) {
            draw_.setFont(u8g2_font_cursor_tf);
            draw_.drawGlyph(OLED_WIDTH - 12, 10, glyph_open_cursor_all_recording);
        }
        else {
            draw_.setFont(u8g2_font_battery19_tn);
            draw_.drawGlyph(OLED_WIDTH - 12, 20, glyph_battery19[sizeof(glyph_battery19) - 1]);
        }
    }
    else {
        draw_.setFont(u8g2_font_battery19_tn);
        draw_.drawGlyph(OLED_WIDTH - 12, 20, glyph_battery19[sizeof(glyph_battery19) - 1]);
    }

    if (data.network.enabled) {
        draw_.setFont(u8g2_font_open_iconic_all_2x_t);
        auto interval = data.network.connected ? 10000 : 1000;
        if (toggle_every(data.time, interval)) {
            draw_.drawGlyph(OLED_WIDTH - 48, 18, glyph_open_iconic_all_wifi_1);
        }
        else {
            draw_.drawGlyph(OLED_WIDTH - 48, 18, glyph_open_iconic_all_wifi_2);
        }
    }

    if (data.gps.enabled) {
        draw_.setFont(u8g2_font_open_iconic_all_2x_t);
        auto interval = data.gps.fix ? 10000 : 1000;
        if (toggle_every(data.time, interval)) {
            draw_.drawGlyph(OLED_WIDTH - 34, 18, glyph_open_iconic_all_gps);
        }
    }

    auto top = 18;
    auto max_h = 20;
    auto rendered_workers = false;
    if (data.primary != nullptr && data.secondary != nullptr) {
        max_h = 16;
    }

    for (auto &info : data.workers) {
        if (info.visible) {
            draw_string_auto_sized(draw_, false, 0, top, OLED_WIDTH, max_h, info.name);
            top += max_h + 6;
            max_h -= 2;

            auto width = (u8g2_uint_t)(OLED_WIDTH * info.progress);
            draw_.drawHLine(0, top, width);

            rendered_workers = true;
        }
    }

    if (!rendered_workers) {
        if (data.primary != nullptr) {
            draw_string_auto_sized(draw_, false, 0, top, OLED_WIDTH, max_h, data.primary);
            top += max_h + 6;
            max_h -= 2;
        }
        if (data.secondary != nullptr) {
            draw_string_auto_sized(draw_, false, 0, top, OLED_WIDTH, max_h, data.secondary);
        }
    }

    draw_.sendBuffer();
}

void U8g2Display::menu(MenuScreen const &data) {
    draw_.setPowerSave(0);
    draw_.clearBuffer();

    if (data.options != nullptr) {
        draw_.setFontMode(0);
        draw_.setFont(u8g2_font_courR08_tf);
        auto y = 0;
        for (auto i = 0; data.options[i] != nullptr; ++i) {
            auto option = data.options[i];
            if (option->visible()) {
                draw_.drawUTF8(2 + 10, y + 12, option->label());
                if (option->selected()) {
                    draw_.drawBox(2, y + 4, 10, 10);
                }
                y += 12;
            }
        }
    }

    draw_.sendBuffer();
}

static bool xbm_get(uint8_t x, uint8_t y, size_t size, uint8_t *data) {
    if (x < 0 || x >= size || y < 0 || y >= size) {
        return false;
    }
    uint32_t offset = y * size + x;
    return (data[offset >> 3] & (1 << (7 - (offset & 0x07)))) != 0;
}

void U8g2Display::qr(QrCodeScreen const &screen) {
    draw_.setPowerSave(0);
    // draw_.setDrawColor(1);
    draw_.clearBuffer();
    // draw_.drawBox(0, 0, OLED_WIDTH, OLED_HEIGHT);
    // draw_.setDrawColor(0);
    auto x0 = (OLED_WIDTH - (screen.size * 2)) / 2;
    auto y0 = (OLED_HEIGHT - (screen.size * 2)) / 2;
    for (size_t x = 0; x < screen.size; ++x) {
        for (size_t y = 0; y < screen.size; ++y) {
            if (xbm_get(x, y, screen.size, screen.data)) {
                draw_.drawBox(x0 + x * 2, y0 + y * 2, 2, 2);
            }
        }
    }
    draw_.sendBuffer();
    // draw_.setDrawColor(0);
}

void U8g2Display::self_check(SelfCheckScreen const &screen) {
    draw_.setPowerSave(0);
    draw_.clearBuffer();
    draw_.setFontMode(1);
    draw_.setFont(u8g2_font_courR08_tf);

    auto x = 0;
    auto y = 12;
    for (size_t i = 0; screen.checks[i] != nullptr; ++i) {
        auto c = screen.checks[i];
        auto width = draw_.getUTF8Width(c->name);
        if (x + width > OLED_WIDTH) {
            y += 16;
            x = 0;
        }

        if (c->pass) {
            draw_.setDrawColor(1);
            draw_.drawUTF8(x, y, c->name);
        }
        else {
            draw_.setDrawColor(1);
            draw_.drawBox(x - 2, y - 10, width + 4, 14);
            draw_.setDrawColor(0);
            draw_.drawUTF8(x, y, c->name);
        }

        x += width + 10;
    }

    draw_.setDrawColor(1);
    draw_.sendBuffer();
}

void U8g2Display::simple(SimpleScreen &&screen) {
    draw_.setPowerSave(0);
    draw_.clearBuffer();

    auto y = OLED_HEIGHT / 3;

    if (screen.message != nullptr) {
        draw_.setFontMode(1);
        draw_.setFont(u8g2_font_courR08_tf);
        draw_centered(draw_, y, screen.message);
    }

    if (screen.secondary != nullptr) {
        draw_.setFontMode(1);
        draw_.setFont(u8g2_font_courR08_tf);
        draw_centered(draw_, y * 2, screen.secondary);
    }

    draw_.sendBuffer();
}

void U8g2Display::reading(ReadingScreen &screen) {
    draw_.clearBuffer();

    draw_.setFontMode(1);
    draw_.setFont(u8g2_font_courR08_tf);

    draw_.drawUTF8(0, 10, screen.module);
    draw_.drawUTF8(0, 20, screen.sensor);

    draw_.setFont(u8g2_font_courR10_tf);

    char value[16];
    tiny_snprintf(value, sizeof(value), "%.3f", screen.value);
    draw_centered(draw_, 40, value);

    draw_.sendBuffer();
}

void U8g2Display::module_status(ModuleStatusScreen &screen) {
    char bay_info[16];
    tiny_snprintf(bay_info, sizeof(bay_info), "Bay #%d", screen.bay + 1);

    draw_.setPowerSave(0);
    draw_.clearBuffer();

    auto y = OLED_HEIGHT / 4;

    draw_.setFontMode(1);
    draw_.setFont(u8g2_font_courR08_tf);

    draw_centered(draw_, y    , bay_info);
    draw_centered(draw_, y * 2, screen.name);
    draw_centered(draw_, y * 3, screen.message);

    draw_.sendBuffer();
}

void U8g2Display::off() {
    draw_.setPowerSave(1);
}

}

#endif
