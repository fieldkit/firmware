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

set(GTEST_URL ${CMAKE_SOURCE_DIR}/third-party/googletest-release-1.11.0.tar.gz)
set(GTEST_SHA256 SHA256=b4870bf121ff7795ba20d20bcdd8627b8e088f2d1dab299a031c1034eddc93d5)
ExternalProject_Add(
    gtest
    URL ${GTEST_URL}
    URL_HASH ${GTEST_SHA256}
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest
    # Disable install step
    INSTALL_COMMAND ""
)

# Get source and binary directories from CMake project
ExternalProject_Get_Property(gtest source_dir binary_dir)

# Create a libgtest target to be used as a dependency by test programs
add_library(libgtest IMPORTED STATIC GLOBAL)
add_dependencies(libgtest gtest)
set_target_properties(libgtest PROPERTIES
  "IMPORTED_LOCATION" "${binary_dir}/lib/libgtest.a"
  "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
)

# Create a libgmock target to be used as a dependency by test programs
add_library(libgmock IMPORTED STATIC GLOBAL)
add_dependencies(libgmock gtest)
set_target_properties(libgmock PROPERTIES
  "IMPORTED_LOCATION" "${binary_dir}/lib/libgmock.a"
  "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
)

# I couldn't make it work with INTERFACE_INCLUDE_DIRECTORIES
include_directories("${source_dir}/googletest/include"
                    "${source_dir}/googlemock/include")

file(GLOB_RECURSE fk_sources ../../fk/*.cpp ../../fk/*.c ../hosted/test_modules.cpp ../../modules/random/main/*.cpp ../../modules/diagnostics/main/*.cpp)
file(GLOB         fk_tasks ../../fk/tasks/tasks.cpp)

list(FILTER fk_sources EXCLUDE REGEX ".*main.cpp$")
list(FILTER fk_sources EXCLUDE REGEX ".*external.cpp$")
list(FILTER fk_sources EXCLUDE REGEX ".*hal/metal/.*.cpp$")
# TODO: Remove (Wire.h dependency)
list(FILTER fk_sources EXCLUDE REGEX ".*tasks/.*.cpp$")

file(GLOB sources *.cpp ${fk_sources} ${fk_tasks})

add_executable(testall ${sources})

find_package(HttpParser)
target_link_libraries(testall HttpParser)

find_package(arduino-logging)
target_link_libraries(testall arduino-logging)

find_package(ArduinoMDNS)
target_link_libraries(testall ArduinoMDNS)

find_package(lwcron)
target_link_libraries(testall lwcron)

find_package(Protocols)
target_link_libraries(testall app-protocol)
target_link_libraries(testall data-protocol)

find_package(CortexLoading)
target_link_libraries(testall CortexLoading)

find_package(Dhara)
target_link_libraries(testall Dhara)

find_package(Samd51Common)
target_link_libraries(testall Samd51Common)

find_package(ConservifyOS)
target_compile_options(ConservifyOS PUBLIC -DOS_CONFIG_HOOKS_CUSTOM)
target_link_libraries(testall ConservifyOS)

find_package(Phylum)
target_link_libraries(testall phylum)

find_package(ArduinoBase64)
target_link_libraries(testall arduino-base64)

target_include_directories(testall PRIVATE ../../fk ${CMAKE_SOURCE_DIR}/third-party/nonstd ${CMAKE_SOURCE_DIR}/third-party/tl)
target_compile_options(testall PUBLIC -Wall -Wno-variadic-macros -DFK_LORA_ABP)
target_link_libraries(testall libgtest libgmock)
add_test(NAME testall COMMAND testall)
