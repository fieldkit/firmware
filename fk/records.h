#pragma once

#include <fk-data-protocol.h>
#include <fk-app-protocol.h>

#include "common.h"
#include "protobuf.h"

namespace fk {

fk_data_DataRecord fk_data_record_decoding_new(Pool &pool);

fk_data_DataRecord fk_data_record_encoding_new();

fk_app_HttpReply fk_http_reply_encoding();

fk_app_HttpQuery *fk_http_query_prepare_decoding(fk_app_HttpQuery *query, Pool *pool);

fk_app_HttpReply *fk_http_reply_encoding_initialize(fk_app_HttpReply *reply);

}
