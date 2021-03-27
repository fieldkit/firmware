#ifndef ALOGGING_LOG_STREAM_H_INCLUDED
#define ALOGGING_LOG_STREAM_H_INCLUDED

#include <type_traits>

#include "logging.h"

#if defined(ARDUINO)
#else // defined(ARDUINO)
#include <string>
#endif // defined(ARDUINO)

class LogStream {
private:
    const char *facility_{ "Root" };
    LogLevels level_{ LogLevels::INFO };
    uint8_t message_[ArduinoLoggingLineMax];
    uint32_t position_{ 0 };

public:
    LogStream() {
    }

    LogStream(LogLevels level) : level_(level) {
    }

    LogStream(const char *facility, LogLevels level = LogLevels::INFO) : facility_(facility), level_(level) {
    }

    ~LogStream();

public:
    LogStream warnings() const {
        return LogStream{ facility_, LogLevels::WARN };
    }

    LogStream errors() const {
        return LogStream{ facility_, LogLevels::ERROR };
    }

    LogStream trace() const {
        return LogStream{ facility_, LogLevels::TRACE };
    }

public:
    LogStream& printf(const char *f, ...) __attribute__((format(printf, 2, 3)));
    LogStream& print(const char *str);
    LogStream& flush();

public:
    LogStream& operator<<(uint8_t i) {
        return printf("%d", i);
    }

    LogStream& operator<<(uint16_t i) {
        return printf("%d", i);
    }

    LogStream& operator<<(uint32_t i) {
        return printf("%" PRIu32, i);
    }

    LogStream& operator<<(uint64_t i) {
        return printf("%" PRIu64, i);
    }

    LogStream& operator<<(int8_t i) {
        return printf("%d", i);
    }

    LogStream& operator<<(int16_t i) {
        return printf("%d", i);
    }

    LogStream& operator<<(int32_t i) {
        return printf("%" PRId32, i);
    }

    LogStream& operator<<(int64_t i) {
        return printf("%" PRId64, i);
    }

    LogStream& operator<<(void *i) {
        return printf("0x%p", i);
    }

    template<class T = uint32_t, typename std::enable_if<!std::is_same<size_t, T>::value, void*>::type = nullptr>
    LogStream& operator<<(size_t i) {
        return printf("%" PRIu32, (uint32_t)i);
    }

    #if !defined(ARDUINO)
    LogStream& operator<<(std::string &v) {
        return printf("%s", v.c_str());
    }
    #endif

    LogStream& operator<<(double v) {
        return printf("%f", v);
    }

    LogStream& operator<<(float v) {
        return printf("%f", v);
    }

    LogStream& operator<<(const char c) {
        return printf("%c", c);
    }

    LogStream& operator<<(const char *s) {
        return printf("%s", s);
    }
};

class Logger {
private:
    const char *facility_{ "Root" };

public:
    Logger() {
    }

    Logger(const char *facility) : facility_(facility) {
    }

public:
    LogStream begin() const;

    LogStream begin(const char *facility) const;

    LogStream operator() () {
        return begin();
    }

    LogStream operator() (const char *facility) {
        return begin(facility);
    }

};

#endif
