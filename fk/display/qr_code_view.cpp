#if defined(__SAMD51__)
#include <qrcode.h>
#endif

#include "common.h"
#include "display/qr_code_view.h"
#include "hal/board.h"
#include "hal/display.h"
#include "platform.h"
#include "state_ref.h"

namespace fk {

FK_DECLARE_LOGGER("qrcode");

QrCodeView::QrCodeView(WhichQrCode which) : which_(which) {
}

void QrCodeView::show() {
    visible_ = false;
}

void QrCodeView::tick(ViewController *views) {
    if (visible_) {
        return;
    }

    // Version 3 (29x29) LOW ECC
    // Numeric = 127
    // Alphanumeric = 77
    // Bytes = 53
    #if defined(__SAMD51__)
    auto bus = get_board()->i2c_core();
    auto display = get_display();

    QRCode qr;
    uint8_t version = 3;
    uint8_t data[qrcode_getBufferSize(version)];

    switch (which_) {
    case WhichQrCode::DeviceId: {
        fk_serial_number_t sn;
        fk_serial_number_get(&sn);
        qrcode_initBytes(&qr, data, version, ECC_MEDIUM, (uint8_t *)&sn, sizeof(sn));
        loginfo("generating qr: <device id>");
        break;
    }
    case WhichQrCode::Name: {
        auto gs = get_global_state_ro();
        qrcode_initText(&qr, data, version, ECC_MEDIUM, gs.get()->general.name);
        loginfo("generating qr: %s", gs.get()->general.name);
        break;
    }
    }

    QrCodeScreen screen;
    screen.size = qr.size;
    screen.data = data;
    display->qr(screen);
    #endif

    visible_ = true;
}

}
