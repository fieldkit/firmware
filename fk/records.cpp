#include "records.h"

namespace fk {

fk_data_DataRecord fk_data_record_decoding_new(Pool &pool) {
    fk_data_DataRecord record = fk_data_DataRecord_init_default;
    record.metadata.firmware.git.funcs.decode = pb_decode_string;
    record.metadata.firmware.git.arg = (void *)&pool;
    record.metadata.firmware.build.funcs.decode = pb_decode_string;
    record.metadata.firmware.build.arg = (void *)&pool;
    record.metadata.deviceId.funcs.decode = pb_decode_data;
    record.metadata.deviceId.arg = (void *)&pool;
    record.identity.name.funcs.decode = pb_decode_string;
    record.identity.name.arg = (void *)&pool;
    record.readings.sensorGroups.funcs.decode = pb_decode_array;
    record.readings.sensorGroups.arg = (void *)&pool;
    return record;
}

fk_data_DataRecord fk_data_record_encoding_new() {
    fk_data_DataRecord record = fk_data_DataRecord_init_default;
    record.metadata.firmware.git.funcs.encode = pb_encode_string;
    record.metadata.firmware.build.funcs.encode = pb_encode_string;
    record.metadata.deviceId.funcs.encode = pb_encode_data;
    record.identity.name.funcs.encode = pb_encode_string;
    record.readings.sensorGroups.funcs.encode = pb_encode_array;
    record.modules.funcs.encode = pb_encode_array;
    return record;
}

}
