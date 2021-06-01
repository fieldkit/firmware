#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <alogging/alogging.h>

int32_t main(int32_t argc, char **argv) {
    ::testing::InitGoogleMock(&argc, argv);
    ::testing::InitGoogleTest(&argc, argv);

    log_configure_level(LogLevels::NONE);

    for (auto i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--info") == 0) {
            log_configure_level(LogLevels::INFO);
        }
        if (strcmp(argv[i], "--debug") == 0) {
            log_configure_level(LogLevels::DEBUG);
        }
        if (strcmp(argv[i], "--verbose") == 0) {
            log_configure_level(LogLevels::VERBOSE);
        }
    }

    return RUN_ALL_TESTS();
}
