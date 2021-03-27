#ifndef FK_ATLAS_PROTOCOL_INCLUDED
#define FK_ATLAS_PROTOCOL_INCLUDED

#define FK_ATLAS_PROTOCOL_VERSION          0x1
#define FK_ATLAS_PROTOCOL_MAX_MESSAGE      64

#include <pb_encode.h>
#include <pb_decode.h>
#include "fk-atlas.pb.h"

#ifdef ARDUINO_SAMD_ZERO
#include <RingBuffer.h>
#if FK_MODULE_PROTOCOL_MAX_MESSAGE > SERIAL_BUFFER_SIZE
#error Module protocol too large for I2C buffer.
#endif
#endif

#endif
