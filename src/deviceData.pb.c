/* Automatically generated nanopb constant definitions */
/* Generated by nanopb-0.3.7 at Fri Dec 16 16:49:49 2016. */

#include "deviceData.pb.h"

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif



const pb_field_t api_Location_fields[4] = {
    PB_FIELD(  1, FLOAT   , OPTIONAL, STATIC  , FIRST, api_Location, longitude, longitude, 0),
    PB_FIELD(  2, FLOAT   , OPTIONAL, STATIC  , OTHER, api_Location, latitude, longitude, 0),
    PB_FIELD(  3, INT32   , OPTIONAL, STATIC  , OTHER, api_Location, altitude, latitude, 0),
    PB_LAST_FIELD
};

const pb_field_t api_DeviceData_fields[12] = {
    PB_FIELD(  1, BOOL    , OPTIONAL, STATIC  , FIRST, api_DeviceData, motion, motion, 0),
    PB_FIELD(  2, UINT32  , OPTIONAL, STATIC  , OTHER, api_DeviceData, water, motion, 0),
    PB_FIELD(  3, FLOAT   , OPTIONAL, STATIC  , OTHER, api_DeviceData, temperature_celcius, water, 0),
    PB_FIELD(  4, FLOAT   , OPTIONAL, STATIC  , OTHER, api_DeviceData, temperature_fahrenheit, temperature_celcius, 0),
    PB_FIELD(  5, FLOAT   , OPTIONAL, STATIC  , OTHER, api_DeviceData, humidity, temperature_fahrenheit, 0),
    PB_FIELD(  6, MESSAGE , OPTIONAL, STATIC  , OTHER, api_DeviceData, location, humidity, &api_Location_fields),
    PB_FIELD(  7, FLOAT   , OPTIONAL, STATIC  , OTHER, api_DeviceData, level, location, 0),
    PB_FIELD(  8, BOOL    , OPTIONAL, STATIC  , OTHER, api_DeviceData, triggered, level, 0),
    PB_FIELD(  9, BOOL    , OPTIONAL, STATIC  , OTHER, api_DeviceData, actuator_active, triggered, 0),
    PB_FIELD( 10, FLOAT   , OPTIONAL, STATIC  , OTHER, api_DeviceData, actuator_time, actuator_active, 0),
    PB_FIELD(110, UINT32  , REPEATED, CALLBACK, OTHER, api_DeviceData, analog_readings, actuator_time, 0),
    PB_LAST_FIELD
};


/* Check that field information fits in pb_field_t */
#if !defined(PB_FIELD_32BIT)
/* If you get an error here, it means that you need to define PB_FIELD_32BIT
 * compile-time option. You can do that in pb.h or on compiler command line.
 * 
 * The reason you need to do this is that some of your messages contain tag
 * numbers or field sizes that are larger than what can fit in 8 or 16 bit
 * field descriptors.
 */
PB_STATIC_ASSERT((pb_membersize(api_DeviceData, location) < 65536), YOU_MUST_DEFINE_PB_FIELD_32BIT_FOR_MESSAGES_api_Location_api_DeviceData)
#endif

#if !defined(PB_FIELD_16BIT) && !defined(PB_FIELD_32BIT)
/* If you get an error here, it means that you need to define PB_FIELD_16BIT
 * compile-time option. You can do that in pb.h or on compiler command line.
 * 
 * The reason you need to do this is that some of your messages contain tag
 * numbers or field sizes that are larger than what can fit in the default
 * 8 bit descriptors.
 */
PB_STATIC_ASSERT((pb_membersize(api_DeviceData, location) < 256), YOU_MUST_DEFINE_PB_FIELD_16BIT_FOR_MESSAGES_api_Location_api_DeviceData)
#endif


/* @@protoc_insertion_point(eof) */
