#pragma once

#include "circular_buffer.h"

namespace fk {

class log_buffer : public circular_buffer<char> {
public:
    log_buffer(char *buf, size_t size) : circular_buffer<char>{ buf, size } {
        bzero(buf, size);
    }

public:
    using circular_buffer<char>::append;
    using circular_buffer<char>::get;

    struct appender {
    private:
        log_buffer *lb;
        bool overwrote{ false };

    public:
        appender(log_buffer *lb) : lb(lb) {
        }

    public:
        void append(char c) {
            if (!overwrote && lb->peek_head() != 0) {
                overwrote = true;
            }

            lb->append(c);

            if (c == 0) {
                done();
            }
        }

        void done() {
            if (overwrote) {
                auto skipped = 0u;
                while (lb->peek_tail() != 0) {
                    lb->get();
                    skipped++;
                }
                if (skipped > 0) {
                    lb->get();
                }
            }
        }
    };

    appender start() {
        return { this };
    }

    void append(const char *str) {
        auto app = start();

        for (auto p = str; *p; ++p) {
            app.append(*p);
        }

        app.append((char)0);
    }

    size_t get(char *str, size_t size) {
        auto i = begin();
        auto sz = 0;

        if (str != nullptr) {
            str[sz] = 0;
        }

        while (i != end()) {
            auto c = *i;

            if (str != nullptr) {
                str[sz] = c;
            }

            if (c == 0) {
                break;
            }

            ++i;
            ++sz;
        }

        if (sz > 0) {
            ++i;
            if (str != nullptr) {
                skip(i);
                if (empty()) {
                    zero();
                }
            }
        }

        return sz;
    }
};

}
