#pragma once

#if defined(FK_HARDWARE_FULL)

#include "hal/display.h"

#include <U8g2lib.h>

namespace fk {

class U8g2Display : public Display {
private:
    U8G2_SH1106_128X64_NONAME_F_HW_I2C draw_;
    bool available_;

public:
    U8g2Display();

public:
    bool begin() override;
    void on() override;
    void off() override;
    void centered(const xbm_data_t &xbm) override;
    void company_logo() override;
    void fk_logo() override;
    void home(HomeScreen const &screen) override;
    void menu(MenuScreen const &screen) override;
    void qr(QrCodeScreen const &screen) override;
    void self_check(SelfCheckScreen const &screen) override;
    void simple(SimpleScreen &&screen) override;
    void reading(ReadingScreen &screen) override;
    void module_status(ModuleStatusScreen &screen) override;

};

}

#endif
