#pragma once

#include <fk-data-protocol.h>

#include "common.h"
#include "protobuf.h"

namespace fk {

fk_data_DataRecord fk_data_record_decoding_new(Pool &pool);

fk_data_DataRecord fk_data_record_encoding_new();

}
