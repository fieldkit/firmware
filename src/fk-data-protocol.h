#ifndef FK_DATA_PROTOCOL_INCLUDED
#define FK_DATA_PROTOCOL_INCLUDED

#define FK_DATA_PROTOCOL_VERSION           0x1
#define FK_DATA_PROTOCOL_MAX_DATA_MESSAGE  128
#define FK_DATA_PROTOCOL_MAX_MESSAGE       FK_DATA_PROTOCOL_MAX_DATA_MESSAGE + 128

#include <pb_encode.h>
#include <pb_decode.h>
#include "fk-data.pb.h"

#endif
