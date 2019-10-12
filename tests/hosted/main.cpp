#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <alogging/alogging.h>

int32_t main(int32_t argc, char **argv) {
    ::testing::InitGoogleMock(&argc, argv);
    ::testing::InitGoogleTest(&argc, argv);

    log_configure_level(LogLevels::ERROR);

    return RUN_ALL_TESTS();
}
