#pragma once

#include "hal/display.h"
#include "display_views.h"

namespace fk {

/**
 * This includes room for the NULL terminator.
 */
constexpr size_t QrCodeMaximumTextLength = 78;

enum class WhichQrCode {
    Name,
    DeviceId,
};

class QrCodeView : public DisplayView {
private:
    WhichQrCode which_;
    bool visible_{ false };

public:
    QrCodeView(WhichQrCode which = WhichQrCode::Name);

public:
    void show() override;
    void tick(ViewController *views) override;


};

}
