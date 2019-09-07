#pragma once

#include <qrcode.h>

#include "common.h"
#include "hal/display.h"
#include "display_views.h"

namespace fk {

class QrCodeView : public DisplayView {
public:
    void tick(ViewController *views) override {
        auto bus = get_board()->i2c_core();
        auto display = get_display();

        fk_serial_number_t sn;
        fk_serial_number_get(&sn);

        // Version 3 (29x29) LOW ECC
        // Numeric = 127
        // Alphanumeric = 77
        // Bytes = 53
        QRCode qr;
        uint8_t version = 3;
        uint8_t data[qrcode_getBufferSize(version)];
        qrcode_initBytes(&qr, data, version, ECC_LOW, (uint8_t *)&sn, sizeof(sn));

        QrCodeScreen screen;
        screen.size = qr.size;
        screen.data = data;
        display->qr(screen);
    }

};

}
