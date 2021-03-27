#include <gtest/gtest.h>
#include <vector>
#include <alogging/alogging.h>

class LoggingSuite : public ::testing::Test {
protected:
    std::vector<std::string> messages;

protected:
    LoggingSuite();
    virtual ~LoggingSuite();

protected:
    virtual void SetUp();
    virtual void TearDown();

};
