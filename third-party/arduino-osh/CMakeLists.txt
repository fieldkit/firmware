#
#
#

project(arduino-osh)
cmake_minimum_required(VERSION 3.5)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

option(TARGET_M0    "Build M0 binaries" OFF)
option(TARGET_M4    "Build M4 binaries" OFF)
option(TARGET_LINUX "Build Linux binaries" OFF)

enable_testing()

add_subdirectory(examples)
add_subdirectory(test)
