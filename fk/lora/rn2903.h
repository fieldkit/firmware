#pragma once

#include "common.h"
#include "sc16is740.h"
#include "line_reader.h"

#if defined(ARDUINO)

namespace fk {

class Rn2903 {
private:
    SC16IS740 bridge_;
    LineReader<256> line_reader_{ &bridge_ };

public:
    Rn2903(TwoWireWrapper &bud);

public:
    bool begin();
    bool read_line_sync(const char **line, uint32_t to);
    bool simple_query(const char *cmd, const char **line, uint32_t to, ...);
    bool save_state();
    bool provision(const char *app_eui, const char *app_key);
    bool configure_us915(uint8_t fsb);
    bool configure_sf(uint8_t sf);
    bool join(const char *app_eui, const char *app_key);

};

}

#endif
