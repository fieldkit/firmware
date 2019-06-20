#include <gtest/gtest.h>

int32_t main(int32_t argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
