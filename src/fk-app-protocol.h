#ifndef FK_APP_PROTOCOL_INCLUDED
#define FK_APP_PROTOCOL_INCLUDED

#define FK_APP_PROTOCOL_VERSION           0x1
#define FK_APP_PROTOCOL_MAX_DATA_MESSAGE  1024
#define FK_APP_PROTOCOL_MAX_MESSAGE       FK_APP_PROTOCOL_MAX_DATA_MESSAGE + 128

#include <pb_encode.h>
#include <pb_decode.h>
#include "fk-app.pb.h"

#endif
