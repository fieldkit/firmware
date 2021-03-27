#
#
#

project(arduino-osh)
cmake_minimum_required(VERSION 3.5)

set(CMAKE_MODULE_PATH $ENV{CMAKE_MODULE_PATH})

option(TARGET_M0    "Build M0 binaries" OFF)
option(TARGET_M4    "Build M4 binaries" OFF)
option(TARGET_LINUX "Build Linux binaries" OFF)

enable_testing()

add_subdirectory(examples)
add_subdirectory(test)
