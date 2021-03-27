#ifndef ALOGGING_LOGGING_LINUX_H_INCLUDED
#define ALOGGING_LOGGING_LINUX_H_INCLUDED

#ifndef ARDUINO

#include <cinttypes>
#include <ostream>

#ifdef __cplusplus
extern "C" {
#endif

uint32_t millis();

#ifdef __cplusplus
}
#endif

#endif // !ARDUINO

#endif // ALOGGING_LOGGING_LINUX_H_INCLUDED
