#pragma once

#include <U8g2lib.h>

#include "display.h"

namespace fk {

class U8g2Display : public Display {
private:
    U8G2_SH1106_128X64_NONAME_F_HW_I2C draw_;
    bool available_;

public:
    U8g2Display();

public:
    bool begin();
    void on() override;
    void off() override;
    void centered(const xbm_data_t &xbm) override;
    void company_logo() override;
    void fk_logo() override;
    void home(home_screen_t data) override;

};

}
