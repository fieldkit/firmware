#ifndef ALOGGING_PLATFORM_H_INCLUDED
#define ALOGGING_PLATFORM_H_INCLUDED

#include "log_stream.h"
#include "logging_arduino.h"
#include "logging_linux.h"

#ifdef __cplusplus
extern "C" {
#endif

void alogging_platform_abort();

#ifdef __cplusplus
}
#endif

using ostreamtype = LogStream;

ostreamtype sdebug();

ostreamtype sdebug(const char *facility);

ostreamtype serror();

ostreamtype serror(const char *facility);

namespace alogging {

constexpr char endl = '\n';

}

#endif // ALOGGING_PLATFORM_H_INCLUDED
