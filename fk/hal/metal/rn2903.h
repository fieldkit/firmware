#pragma once

#include "common.h"
#include "sc16is740.h"
#include "line_reader.h"
#include "hal/lora.h"

#if defined(ARDUINO)

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
    bool join(const char *app_eui, const char *app_key, int32_t retries = 3, uint32_t retry_delay = 10000);
    bool sleep(uint32_t ms);
    bool wake();
    bool send_bytes(uint8_t const *data, size_t size, uint8_t port);

private:
    static LoraErrorCode translate_error(const char *line);
    bool send_command(const char *cmd, ...);
    bool send_command(const char *cmd, va_list args);
    bool provision(const char *app_eui, const char *app_key);
    bool configure_us915(uint8_t fsb);
    bool configure_sf(uint8_t sf);
    bool save_state();

};

}

#endif
