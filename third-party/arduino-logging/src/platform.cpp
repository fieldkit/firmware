#include "logging.h"
#include "platform.h"

ostreamtype sdebug() {
    return LogStream{ };
}

ostreamtype sdebug(const char *facility) {
    return LogStream{ facility };
}

ostreamtype serror() {
    return LogStream{ LogLevels::ERROR };
}

ostreamtype serror(const char *facility) {
    return LogStream{ facility, LogLevels::ERROR };
}
