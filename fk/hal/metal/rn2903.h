#pragma once

#include "common.h"
#include "hal/lora.h"
#include "line_reader.h"
#include "sc16is740.h"

#if defined(__SAMD51__)

namespace fk {

class Rn2903 {
private:
    Sc16is740 bridge_;
    LineReader<256> line_reader_{ &bridge_ };
    LoraErrorCode error_;

public:
    Rn2903();

public:
    LoraErrorCode error() const {
        return error_;
    };

public:
    bool begin();
    bool read_line_sync(const char **line, uint32_t to, bool quiet = false);
    bool simple_query(const char *cmd, uint32_t to, ...);
    bool simple_query(const char *cmd, const char **line, uint32_t to, ...);
    bool join(LoraOtaaJoin &otaa, int32_t retries = 3, uint32_t retry_delay = 10000);
    bool join(const char *mode);
    bool sleep(uint32_t ms);
    bool wake();
    bool factory_reset();
    bool send_bytes(uint8_t const *data, size_t size, uint8_t port, bool confirmed);
    bool save_state();
    bool disable_adr();

public:
    bool configure_sf(uint8_t sf);

private:
    static LoraErrorCode translate_error(const char *line);
    bool send_command(const char *cmd, ...);
    bool send_command(const char *cmd, va_list args);
    bool provision(LoraOtaaJoin &otaa);
    bool provision(const char *app_session_key, const char *network_session_key, const char *device_address, uint32_t uplink_counter,
                   uint32_t downlink_counter);
    bool configure_radio(LoraOtaaJoin &otaa);
    bool configure_us915(uint8_t fsb);
    bool configure_eu868(uint8_t fsb);
};

} // namespace fk

#endif
