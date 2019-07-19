#
#
#

include(${CMAKE_SOURCE_DIR}/libraries/dependencies.cmake)

set(CMAKE_BUILD_TYPE DEBUG)

find_package(Threads REQUIRED)

include(ExternalProject)

file(GLOB_RECURSE fk_sources ../../fk/*.cpp ../../fk/*.c)
list(FILTER fk_sources EXCLUDE REGEX ".*main.cpp$")
list(FILTER fk_sources EXCLUDE REGEX ".*external.cpp$")
# TODO: Remove
list(FILTER fk_sources EXCLUDE REGEX ".*self_check.cpp$")
# TODO: Remove (Wire.h dependency)
list(FILTER fk_sources EXCLUDE REGEX ".*temperature.cpp$")
list(FILTER fk_sources EXCLUDE REGEX ".*battery_gauge.cpp$")
list(FILTER fk_sources EXCLUDE REGEX ".*clock.cpp$")
list(FILTER fk_sources EXCLUDE REGEX ".*tasks/.*.cpp$")

file(GLOB sources *.cpp ${fk_sources})

add_executable(fake ${sources})

find_package(lwstreams)
target_link_libraries(fake lwstreams)

find_package(HttpParser)
target_link_libraries(fake HttpParser)

find_package(arduino-logging)
target_link_libraries(fake arduino-logging)

find_package(Protocols)
target_link_libraries(fake app-protocol)
target_link_libraries(fake data-protocol)

find_package(CortexLoading)
target_link_libraries(fake CortexLoading)

find_package(ConservifyOS)
target_link_libraries(fake ConservifyOS)

find_package(Phylum)
target_link_libraries(fake phylum)

target_include_directories(fake PRIVATE ../../fk)
target_compile_options(fake PUBLIC -Wall -Wno-variadic-macros)
target_link_libraries(fake ${CMAKE_THREAD_LIBS_INIT})
