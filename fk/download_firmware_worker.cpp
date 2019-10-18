#include <loading.h>

#include "download_firmware_worker.h"

#include "hal/network.h"
#include "platform.h"

namespace fk {

FK_DECLARE_LOGGER("fwdownload");

DownloadFirmwareWorker::DownloadFirmwareWorker() {
}

void DownloadFirmwareWorker::run(Pool &pool) {
    auto cw = get_network()->open_connection("192.168.0.100", 6060);
    if (cw == nullptr) {
        logerror("connection error");
        return;
    }

    loginfo("connected!");

    auto c = cw->get();

    auto test = pool.sprintf("GET /public/fk-bundled-fkb.bin HTTP/1.1\r\n"
                             "Host: %s:%d\r\n"
                             "Connection: close\r\n"
                             "Accept: */*\r\n\r\n", "192.168.0.100", 6060);

    c->write(test);

    auto started = fk_uptime();
    while (fk_uptime() - started < 5 * 1000) {
        uint8_t buffer[512];
        auto nread = c->read(buffer, sizeof(buffer) - 1);
        if (nread > 0) {
            buffer[nread] = 0;
            loginfo("read: %" PRId32, nread);
            fkb_external_printf("%s\n", buffer);
        }
        if (nread == 0) {
            break;
        }
    }

    c->stop();

    delete cw;
}

}
