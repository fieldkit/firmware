#include "modules/bridge/modules_bridge.h"

#include "records.h"

namespace fk {

size_t log_bytes(const char *prefix, uint8_t const *ptr, size_t size) {
    auto bytes_per_line = 32u;
    auto remaining = size;
    char hex_size = bytes_per_line * 2 + 1;
    char hex[hex_size];

    for (auto pos = 0u; pos < size;) {
        auto logging = std::min<int32_t>(bytes_per_line, remaining);

        bytes_to_hex_string(hex, hex_size, ptr + pos, logging);

        alogf(LogLevels::INFO, "%s %s", prefix, hex);

        remaining -= logging;
        pos += logging;
    }

    return size;
}

std::pair<EncodedMessage *, fk_data_ModuleConfiguration *> Module::read_configuration_eeprom(ModuleEeprom &eeprom, Pool *pool) {
    size_t size = 0;
    uint8_t *buffer = nullptr;
    if (eeprom.read_configuration(&buffer, size, pool)) {
        log_bytes("modcfg", buffer, size);

        auto stream = pb_istream_from_buffer(buffer, size);
        auto cfg = fk_module_configuration_decoding_new(pool);
        if (!pb_decode_delimited(&stream, fk_data_ModuleConfiguration_fields, cfg)) {
            alogf(LogLevels::WARN, "mod-cfg", "decode error");
        } else {
            return { pool->wrap(buffer, size), cfg };
        }
    }

    return { nullptr, nullptr };
}

} // namespace fk
