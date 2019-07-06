#include <gtest/gtest.h>
#include <gmock/gmock.h>

int32_t main(int32_t argc, char **argv) {
    ::testing::InitGoogleMock(&argc, argv);
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
