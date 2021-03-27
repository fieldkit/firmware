#if defined(ARDUINO_LOGGING_CSTDIO)

#include <cstdio>
#include <cstring>

#define alogging_snprintf   snprintf
#define alogging_vsnprintf  vsnprintf

#else

#include <cstring>

#include "../printf.h"

#endif // defined(ARDUINO_LOGGING_CSTDIO)

