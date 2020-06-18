#include "tests.h"

#include "log_buffer.h"

using namespace fk;

static void dump_logs(log_buffer &logs) __attribute__((unused));
static void dump_buffer(const char *buffer, size_t size) __attribute__((unused));

class CircularBufferSuite : public ::testing::Test {
};

TEST_F(CircularBufferSuite, Empty) {
    char buffer[16];
    log_buffer logs{ buffer, sizeof(buffer) };

    logs.zero();

    char message[16];
    ASSERT_EQ(logs.get(message, sizeof(message)), 0u);
}

TEST_F(CircularBufferSuite, Basic) {
    char buffer[16];
    log_buffer logs{ buffer, sizeof(buffer) };

    logs.zero();

    logs.append("Jacob1");
    logs.append("Jacob2");

    char message[16];

    auto bytes0 = logs.get(message, sizeof(message));
    ASSERT_EQ(bytes0, 6u);
    ASSERT_STREQ(message, "Jacob1");

    auto bytes1 = logs.get(message, sizeof(message));
    ASSERT_EQ(bytes1, 6u);
    ASSERT_STREQ(message, "Jacob2");

    auto bytes2 = logs.get(message, sizeof(message));
    ASSERT_EQ(bytes2, 0u);
    ASSERT_STREQ(message, "");
}

TEST_F(CircularBufferSuite, AppendingWrappingRead) {
    char buffer[16];
    log_buffer logs{ buffer, sizeof(buffer) };

    logs.zero();

    logs.append("Jacob1");
    logs.append("Jacob2");
    logs.append("Jacob3");

    char message[64];

    auto bytes0 = logs.get(message, sizeof(message));
    ASSERT_STREQ(message, "Jacob2");
    ASSERT_EQ(bytes0, 6u);

    auto bytes1 = logs.get(message, sizeof(message));
    ASSERT_STREQ(message, "Jacob3");
    ASSERT_EQ(bytes1, 6u);

    auto bytes2 = logs.get(message, sizeof(message));
    ASSERT_STREQ(message, "");
    ASSERT_EQ(bytes2, 0u);
}

TEST_F(CircularBufferSuite, AppendingReadWrappingRead) {
    char message[64];
    char buffer[16];
    log_buffer logs{ buffer, sizeof(buffer) };

    logs.zero();

    logs.append("Jacob1");

    logs.get(message, sizeof(message));
    ASSERT_STREQ(message, "Jacob1");

    logs.append("Jacob2");
    logs.append("Jacob3");
    logs.append("Jacob4");

    logs.get(message, sizeof(message));
    ASSERT_STREQ(message, "Jacob3");

    logs.get(message, sizeof(message));
    ASSERT_STREQ(message, "Jacob4");

    logs.get(message, sizeof(message));
    ASSERT_STREQ(message, "");
}

TEST_F(CircularBufferSuite, AppendingAndIterating) {
    static_log_buffer<16> logs;

    logs.zero();

    logs.append("Jacob1");

    auto size = 0u;
    auto real = 0u;
    for (auto c : logs) {
        if (c != 0) {
            size++;
        }
        real++;
    }

    ASSERT_EQ(size, 6u);
    ASSERT_EQ(real, 7u);
}

TEST_F(CircularBufferSuite, WrappingAndIterating) {
    static_log_buffer<16> logs;

    logs.zero();

    logs.append("Jacob1");
    logs.append("Jacob2");
    logs.append("Jacob3");

    auto size = 0u;
    auto real = 0u;
    for (auto c : logs) {
        if (c != 0) {
            size++;
        }
        real++;
    }

    ASSERT_EQ(size, 12u);
    ASSERT_EQ(real, 14u);
}

template<typename T>
static void read_until_end(T &iter, T end, char *buffer, size_t sz) {
    auto index = 0u;
    for (; iter != end; ++iter) {
        if (index < sz - 1) {
            if (*iter != 0) {
                buffer[index++] = *iter;
            }
        }
    }
    buffer[index++] = 0;
}

TEST_F(CircularBufferSuite, ResumingIterators) {
    char message[128];
    char buffer[128];
    log_buffer logs{ buffer, sizeof(buffer) };

    logs.zero();

    auto iter1 = logs.tail();
    auto iter2 = logs.tail();

    logs.append("Jacob1");
    logs.append("Jacob2");

    read_until_end(iter1, logs.end(), message, sizeof(message));

    ASSERT_STREQ(message, "Jacob1Jacob2");

    logs.append("Jacob3");

    read_until_end(iter1, logs.end(), message, sizeof(message));

    ASSERT_STREQ(message, "Jacob3");

    read_until_end(iter2, logs.end(), message, sizeof(message));

    ASSERT_STREQ(message, "Jacob1Jacob2Jacob3");
}

static void dump_buffer(const char *buffer, size_t size) {
    printf("'");
    for (auto i = 0u; i < size; ++i) {
        auto c = buffer[i];
        if (c == 0) {
            printf("(nil)");
        }
        else {
            printf("%c", c);
        }
    }
    printf("'\n");
}

static void dump_logs(log_buffer &logs) {
    printf("'");
    for (auto c : logs) {
        if (c == 0) {
            printf("(nil)");
        }
        else {
            printf("%c", c);
        }
    }
    printf("'\n");
}
