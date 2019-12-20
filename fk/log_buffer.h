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

    void append(const char *str) {
        auto overwrote = false;

        for (auto p = str; *p; ++p) {
            if (!overwrote && peek_head() != 0) {
                overwrote = true;
            }
            append(*p);
        }

        append((char)0);

        // Consume any message we overwrote;
        if (overwrote) {
            auto skipped = 0u;
            while (peek_tail() != 0) {
                get();
                skipped++;
            }
            if (skipped > 0) {
                get();
            }
        }
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
