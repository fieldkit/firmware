#
#
#
project(fk)
cmake_minimum_required(VERSION 3.6)

set(CMAKE_MODULE_PATH $ENV{CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

message("Target Architecture: ${TARGET_ARCH}")

enable_testing()

add_subdirectory(libraries)

add_subdirectory(tests)

add_subdirectory(boards)

if(${TARGET_ARCH} MATCHES "samd51")
  add_subdirectory(bootloader)
  add_subdirectory(fk)
endif()
