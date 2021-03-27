#include "test_logging.h"

LoggingSuite::LoggingSuite() {
}

LoggingSuite::~LoggingSuite() {
};

using namespace alogging;

class Example {
public:
    friend LogStream& operator<<(LogStream &log, const Example &example) {
        return log << "Example<>";
    }
};

static size_t LogMessageHook(const LogMessage *m, const char *fstring, va_list args, void *arg) {
    char buffer[256] = { 0 };

    va_list copy;
    va_copy(copy, args);

    auto prefix = snprintf(buffer, sizeof(buffer) - 1, "%06" PRIu32 " %-25s ", m->uptime, m->facility);
    auto full = vsnprintf(buffer + prefix, sizeof(buffer) - 1 - prefix, fstring, copy);
    if (prefix + full + 1 >= sizeof(buffer)) {
        buffer[sizeof(buffer) - 2] = '\n';
        buffer[sizeof(buffer) - 1] = 0;
    }
    else {
        buffer[prefix + full] = '\n';
        buffer[prefix + full + 1] = 0;
    }

    std::vector<std::string> *messages = (std::vector<std::string> *)arg;
    messages->emplace_back(buffer);
    return 0;
}

void LoggingSuite::SetUp() {
    log_configure_hook_register(LogMessageHook, (void *)&messages);
    log_configure_time(nullptr, nullptr);
};

void LoggingSuite::TearDown() {
    log_configure_hook_register(nullptr, nullptr);
};

TEST_F(LoggingSuite, Simple) {
    alogf(LogLevels::INFO, "Simple", "Hello World");

    EXPECT_EQ(messages.size(), 1);
    EXPECT_EQ(messages[0], "000000 Simple                    Hello World\n");
}

TEST_F(LoggingSuite, DisableHook) {
    alogf(LogLevels::INFO, "Simple", "Hello World");

    log_configure_hook(false);

    alogf(LogLevels::INFO, "Simple", "Bye World");

    EXPECT_EQ(messages.size(), 1);
    EXPECT_EQ(messages[0], "000000 Simple                    Hello World\n");
}

TEST_F(LoggingSuite, LogStream) {
    Logger logger("Root");
    logger() << "Hello" << " " << "World";

    logger() << "Hello" << " " << "World " << 100 << " " << 3.14;

    EXPECT_EQ(messages.size(), 2);
    EXPECT_EQ(messages[0], "000000 Root                      Hello World\n");
    EXPECT_EQ(messages[1], "000000 Root                      Hello World 100 3.140000\n");

    auto example = Example{};
    logger() << "Hello" << " " << example;
    EXPECT_EQ(messages.size(), 3);
    EXPECT_EQ(messages[2], "000000 Root                      Hello Example<>\n");

    logger.begin() << "Hello " << example;
    EXPECT_EQ(messages.size(), 4);
    EXPECT_EQ(messages[3], "000000 Root                      Hello Example<>\n");

    logger() << "Hello " << example;
    EXPECT_EQ(messages.size(), 5);
    EXPECT_EQ(messages[4], "000000 Root                      Hello Example<>\n");
}

TEST_F(LoggingSuite, EmptyMessage) {
    Logger logger("Root");

    logger() << "";

    EXPECT_EQ(messages.size(), 0);
}

TEST_F(LoggingSuite, TooLongMessage) {
    Logger logger("Root");
    {
        auto e = logger.begin();
        for (auto i = 0; i < 60 ; ++i) {
            e << "Jacob";
        }
    }

    EXPECT_EQ(messages.size(), 1);
    EXPECT_EQ(messages[0], "000000 Root                      JacobJacobJacobJacobJacobJacobJacobJacobJacobJacobJacobJacobJacobJacobJacobJacobJacobJacobJacobJacobJacobJacobJacobJacobJacobJacobJacobJacobJacobJacobJacobJacobJacobJacobJacobJacobJacobJacobJacobJacobJacobJacobJacobJacobJ\n");
}

TEST_F(LoggingSuite, Sdebug) {
    sdebug() << "Hello, world!" << endl;
}

TEST_F(LoggingSuite, AssertFail) {
}
