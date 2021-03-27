#ifndef ALOGGING_SIMPLE_LOG_H_INCLUDED
#define ALOGGING_SIMPLE_LOG_H_INCLUDED

#include "logging.h"

// NOTE: This is a C++11 feature.
template<const char *Name>
class SimpleLog {
public:
    static void log(const char *f, ...) __attribute__((format(printf, 1, 2))) {
        va_list args;
        va_start(args, f);
        valogf(LogLevels::INFO, Name, f, args);
        va_end(args);
    }

    static void info(const char *f, ...) __attribute__((format(printf, 1, 2))) {
        va_list args;
        va_start(args, f);
        valogf(LogLevels::INFO, Name, f, args);
        va_end(args);
    }

    static void trace(const char *f, ...) __attribute__((format(printf, 1, 2))) {
        va_list args;
        va_start(args, f);
        valogf(LogLevels::TRACE, Name, f, args);
        va_end(args);
    }

    static void warn(const char *f, ...) __attribute__((format(printf, 1, 2))) {
        va_list args;
        va_start(args, f);
        valogf(LogLevels::WARN, Name, f, args);
        va_end(args);
    }

    static void error(const char *f, ...) __attribute__((format(printf, 1, 2))) {
        va_list args;
        va_start(args, f);
        valogf(LogLevels::ERROR, Name, f, args);
        va_end(args);
    }

};

#endif
