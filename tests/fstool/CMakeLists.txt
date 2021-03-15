#
#
#

include(${CMAKE_SOURCE_DIR}/fk/dependencies.cmake)

set(CMAKE_BUILD_TYPE DEBUG)

find_program(CCACHE_PROGRAM ccache)

if(CCACHE_PROGRAM)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

find_package(Threads REQUIRED)

include(ExternalProject)

file(GLOB_RECURSE fk_sources ../../fk/*.cpp ../../fk/*.c ../hosted/test_modules.cpp ../../third-party/arduino-stubs/*.cpp)
file(GLOB         fk_tasks ../../fk/tasks/tasks.cpp)

list(FILTER fk_sources EXCLUDE REGEX ".*main.cpp$")
list(FILTER fk_sources EXCLUDE REGEX ".*external.cpp$")
list(FILTER fk_sources EXCLUDE REGEX ".*hal/metal/.*.cpp$")
# TODO: Remove (Wire.h dependency)
list(FILTER fk_sources EXCLUDE REGEX ".*tasks/.*.cpp$")

file(GLOB sources *.cpp *.c ${fk_sources} ${fk_tasks})

add_executable(fstool ${sources})

find_package(HttpParser)
target_link_libraries(fstool HttpParser)

find_package(arduino-logging)
target_link_libraries(fstool arduino-logging)

find_package(lwcron)
target_link_libraries(fstool lwcron)

find_package(Protocols)
target_link_libraries(fstool app-protocol)
target_link_libraries(fstool data-protocol)

find_package(CortexLoading)
target_link_libraries(fstool CortexLoading)

find_package(ConservifyOS)
target_link_libraries(fstool ConservifyOS)

find_package(Lfs)
target_link_libraries(fstool Lfs)

find_package(Dhara)
target_link_libraries(fstool Dhara)

find_package(Samd51Common)
target_link_libraries(fstool Samd51Common)

find_package(Phylum)
target_link_libraries(fstool phylum)

find_package(ArduinoBase64)
target_link_libraries(fstool arduino-base64)

target_include_directories(fstool PRIVATE ../../fk ../hosted ${CMAKE_SOURCE_DIR}/third-party/nonstd ${CMAKE_SOURCE_DIR}/third-party/tl)
target_compile_options(fstool PUBLIC -Wall -Werror -Wno-variadic-macros)
target_link_libraries(fstool ${CMAKE_THREAD_LIBS_INIT})
