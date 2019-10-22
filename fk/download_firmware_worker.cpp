#include <loading.h>
#include <phylum/blake2b.h>

#include "download_firmware_worker.h"

#include "firmware_manager.h"
#include "networking/cpool.h"
#include "storage/types.h"
#include "storage/progress_tracker.h"
#include "hal/network.h"
#include "hal/flash.h"
#include "progress.h"
#include "utilities.h"
#include "state_manager.h"
#include "platform.h"

namespace fk {

FK_DECLARE_LOGGER("fwdownload");

class HttpConnection {
private:
    PoolWrapper<NetworkConnection> *nc_;
    Connection *connection_;

public:
    HttpConnection(PoolWrapper<NetworkConnection> *nc) : nc_(nc) {
        connection_ = new (nc_->pool()) Connection(nc_->pool(), nc_->get(), 0);
    }

    virtual ~HttpConnection() {
        nc_->get()->stop();
        delete nc_;
    }

public:
    bool head(const char *path, const char *server, uint16_t port) {
        return begin("HEAD", path, server, port);
    }

    bool get(const char *path, const char *server, uint16_t port) {
        return begin("GET", path, server, port);
    }

public:
    bool ignore_body() {
        while (fk_uptime() - connection_->activity() < NetworkConnectionMaximumDuration) {
            if (!connection_->service()) {
                break;
            }

            if (connection_->have_headers()) {
                loginfo("have headers");
                break;
            }
        }

        connection_->close();

        return true;
    }

    bool copy(Pool &pool) {
        auto buffer = (uint8_t *)nc_->pool().malloc(CodeMemoryPageSize);
        auto total_read = 0u;
        auto success = false;

        BLAKE2b b2b;
        b2b.reset(Hash::Length);

        NoopProgressCallbacks noop;
        auto tracker = ProgressTracker{ &noop, Operation::Fsck, "download", "", connection_->length() };
        auto eeprom_address = OtherBankAddress + BootloaderSize;
        auto position = 0u;

        while (true) {
            if (!connection_->service()) {
                logwarn("disconnected");
                break;
            }

            if (connection_->have_headers()) {
                auto reading = CodeMemoryPageSize - position;
                auto nread = connection_->read(buffer + position, reading);
                if (nread > 0) {
                    tracker.update(nread, connection_->length());
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
            }

            if (fk_uptime() - connection_->activity() > NetworkConnectionMaximumDuration) {
                logwarn("inactive");
                break;
            }

            fk_delay(1);
        }

        if (success) {
            loginfo("success!");
        }

        connection_->close();

        return success;
    }

public:
    uint32_t length() const {
        return connection_->length();
    }

private:
    bool begin(const char *method, const char *path, const char *server, uint16_t port) {
        connection_->printf("%s %s HTTP/1.1\r\n"
                            "Host: %s:%d\r\n"
                            "Connection: close\r\n"
                            "Accept: */*\r\n\r\n", method, path, server, port);
        return true;
    }

};

DownloadFirmwareWorker::DownloadFirmwareWorker() {
}

void DownloadFirmwareWorker::run(Pool &pool) {
    auto path = "/fk-bundled-fkb.bin";
    auto server = "192.168.0.100";
    auto port = 6060;

    loginfo("backup bootloader...");

    FirmwareManager firmware;
    firmware.backup_bootloader(pool);

    loginfo("connecting to '%s:%d'", server, port);

    auto nc = get_network()->open_connection(server, port);
    if (nc == nullptr) {
        logerror("connection error");
        return;
    }

    loginfo("connected!");

    if (false) {
        HttpConnection http_head{ nc };
        if (!http_head.head(path, server, port)) {
            return;
        }

        if (!http_head.ignore_body()) {
            return;
        }

        loginfo("size: %" PRIu32, http_head.length());
    }

    GlobalStateManager gsm;

    HttpConnection http_get{ nc };
    if (!http_get.get(path, server, port)) {
        gsm.notify({ "error!" });
        return;
    }

    if (!http_get.copy(pool)) {
        gsm.notify({ "error!" });
        return;
    }

    gsm.notify({ "success, swap!" });
}

}
