#pragma once

#include "common.h"
#include "sc16is740.h"
#include "line_reader.h"

#if defined(ARDUINO)

namespace fk {

class Rn2903 {
private:
    Sc16is740 bridge_;
    LineReader<256> line_reader_{ &bridge_ };

public:
    Rn2903(TwoWireWrapper &bus);

public:
    bool begin();
    bool read_line_sync(const char **line, uint32_t to);
    bool simple_query(const char *cmd, uint32_t to, ...);
    bool simple_query(const char *cmd, const char **line, uint32_t to, ...);
    bool join(const char *app_eui, const char *app_key, int32_t retries = 3, uint32_t retry_delay = 10000);

private:
    bool send_command(const char *cmd, uint32_t to, va_list args);
    bool provision(const char *app_eui, const char *app_key);
    bool configure_us915(uint8_t fsb);
    bool configure_sf(uint8_t sf);
    bool save_state();

};

}

#endif
