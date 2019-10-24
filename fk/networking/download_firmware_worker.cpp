#include <loading.h>
#include <samd51_common.h>
#include <phylum/blake2b.h>

#include "networking/download_firmware_worker.h"

#include "firmware_manager.h"
#include "networking/cpool.h"
#include "storage/types.h"
#include "progress.h"
#include "progress_tracker.h"
#include "gs_progress_callbacks.h"
#include "hal/network.h"
#include "hal/flash.h"
#include "utilities.h"
#include "state_manager.h"
#include "platform.h"

#include "io.h"

namespace fk {

FK_DECLARE_LOGGER("fwdownload");

class HttpConnection : public Reader, public Writer {
private:
    PoolPointer<NetworkConnection> *nc_;
    Connection *connection_;

public:
    HttpConnection(PoolPointer<NetworkConnection> *nc) : nc_(nc) {
        connection_ = new (nc_->pool()) Connection(nc_->pool(), nc_->get(), 0);
    }

    virtual ~HttpConnection() {
        close();
    }

public:
    int32_t write(uint8_t const *buffer, size_t size) override {
        if (!connection_->active()) {
            return -1;
        }

        if (!connection_->service()) {
            return -1;
        }

        return connection_->write(buffer, size);
    }

    int32_t read(uint8_t *buffer, size_t size) override {
        if (!connection_->active()) {
            return -1;
        }

        if (!connection_->service()) {
            return -1;
        }

        return connection_->read(buffer, size);
    }

public:
    bool begin(const char *method, const char *path, const char *server, uint16_t port) {
        connection_->printf("%s /%s HTTP/1.1\r\n"
                            "Host: %s:%d\r\n"
                            "Connection: close\r\n"
                            "Accept: */*\r\n\r\n", method, path, server, port);

        while (connection_->active()) {
            if (!connection_->service()) {
                break;
            }

            if (connection_->have_headers()) {
                return true;
            }
        }

        close();

        return false;
    }

    void close() {
        if (nc_ != nullptr) {
            connection_->close();
            nc_->get()->stop();
            delete nc_;
            nc_ = nullptr;
        }
    }

public:
    uint32_t length() const {
        return connection_->length();
    }

    bool active() const {
        return connection_->active();
    }

};

HttpConnection *open_http_connection(const char *method, const char *url, Pool &pool) {
    UrlParser url_parser{ pool.strdup(url) };

    loginfo("connecting to: %s:%d", url_parser.server(), url_parser.port());

    auto nc = get_network()->open_connection(url_parser.server(), url_parser.port());
    if (nc == nullptr) {
        logerror("connection error");
        return nullptr;
    }

    auto http = new (pool) HttpConnection(nc);

    loginfo("beginning %s %s", method, url_parser.path());

    if (!http->begin(method, url_parser.path(), url_parser.server(), url_parser.port())) {
        http->close();
        return nullptr;
    }

    return http;
}

DownloadFirmwareWorker::DownloadFirmwareWorker() {
}

void DownloadFirmwareWorker::run(Pool &pool) {
    GlobalStateManager gsm;
    gsm.notify({ "downloading" });

    loginfo("backup bootloader...");
    FirmwareManager firmware;
    firmware.backup_bootloader(pool);

    auto url = "http://192.168.0.100:6060/fk-bundled-fkb.bin";
    auto http = open_http_connection("GET", url, pool);
    if (http == nullptr) {
        return;
    }

    loginfo("connected!");

    GlobalStateProgressCallbacks progress;

    auto tracker = ProgressTracker{ &progress, Operation::Download, "download", "", http->length() };
    auto buffer = (uint8_t *)pool.malloc(CodeMemoryPageSize);
    auto eeprom_address = OtherBankAddress + BootloaderSize;
    auto total_read = 0u;
    auto success = false;
    auto position = 0u;

    BLAKE2b b2b;
    b2b.reset(Hash::Length);

    while (true) {
        auto reading = CodeMemoryPageSize - position;
        auto nread = http->read(buffer + position, reading);
        if (nread < 0) {
            break;
        }
        if (nread > 0) {
            tracker.update(nread, http->length());
            total_read += nread;
            position += nread;

            if (position == CodeMemoryPageSize || tracker.done()) {
                if (eeprom_address % CodeMemoryBlockSize == 0) {
                    loginfo("[0x%06" PRIx32 "] erasing", eeprom_address);
                    get_flash()->erase(eeprom_address, CodeMemoryBlockSize / CodeMemoryPageSize);
                }

                // Don't include the hash in the actual hash, it's at
                // the tail end of the file. So, if we're done we've got
                // everything so hash up to the beginning of the hash.
                if (tracker.done()) {
                    b2b.update(buffer, position - Hash::Length);
                }
                else {
                    b2b.update(buffer, position);
                }

                get_flash()->write(eeprom_address, buffer, position);
                eeprom_address += position;
                position = 0;
            }

            if (tracker.done()) {
                Hash hash;
                b2b.finalize(&hash.hash, Hash::Length);

                // Compare the hash we calculated with the one that was
                // just written to memory.
                auto eeprom_hash_ptr = reinterpret_cast<uint8_t*>(eeprom_address - Hash::Length);
                if (memcmp(eeprom_hash_ptr, hash.hash, Hash::Length) != 0) {
                    logerror("hash mismatch!");
                    fk_dump_memory("EXP ", eeprom_hash_ptr, Hash::Length);
                    fk_dump_memory("ACT ", hash.hash, Hash::Length);
                }
                else {
                    auto hex_str = bytes_to_hex_string_pool(hash.hash, Hash::Length, pool);
                    loginfo("hash is good: %s", hex_str);
                    success = true;
                }

                break;
            }
        }

        fk_delay(1);
    }

    http->close();

    if (!success) {
        gsm.notify({ "error!" });
        logwarn("http get of %s failed", url);
        return;
    }

    gsm.notify({ "success!" });

    loginfo("success!");

    if (!fk_debug_get_console_attached()) {
        fk_nvm_swap_banks();
    }
}

}
