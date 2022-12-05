#include "modules/bridge/modules_bridge.h"

#include "records.h"

namespace fk {

std::pair<EncodedMessage *, fk_data_ModuleConfiguration *> Module::read_configuration_eeprom(ModuleEeprom &eeprom, Pool *pool) {
    size_t size = 0;
    auto buffer = (uint8_t *)pool->malloc(MaximumConfigurationSize);
    bzero(buffer, MaximumConfigurationSize);
    if (eeprom.read_configuration(buffer, size, MaximumConfigurationSize)) {
        auto cfg = fk_module_configuration_decoding_new(pool);
        auto stream = pb_istream_from_buffer(buffer, size);
        if (!pb_decode_delimited(&stream, fk_data_ModuleConfiguration_fields, cfg)) {
            alogf(LogLevels::WARN, "mod-cfg", "decode error");
        } else {
            return { pool->wrap(buffer, size), cfg };
        }
    }

    return { nullptr, nullptr };
}

} // namespace fk
