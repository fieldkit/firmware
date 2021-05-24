set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_CXX_STANDARD 14)

find_package(Threads REQUIRED)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/third-party/arduino-logging/cmake ${CMAKE_SOURCE_DIR}/cmake)

include(ExternalProject)

set(GTEST_URL ${CMAKE_SOURCE_DIR}/third-party/googletest-release-1.10.0.tar.gz)
set(GTEST_SHA256 SHA256=9dc9157a9a1551ec7a7e43daea9a694a0bb5fb8bec81235d8a1e6ef64c716dcb)

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
include_directories("${source_dir}/googletest/include" "${source_dir}/googlemock/include")

file(GLOB library_sources ${CMAKE_SOURCE_DIR}/src/*.cpp)
file(GLOB test_sources *.cpp)

add_executable(testall ${library_sources} ${test_sources})

target_include_directories(testall PRIVATE ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/tests)
target_compile_options(testall PRIVATE -Wall -fstack-usage)

find_package(ArduinoLogging)
target_link_libraries(testall ArduinoLogging)

find_package(Dhara)
target_link_libraries(testall Dhara)

target_link_libraries(testall libgtest libgmock)
add_test(NAME testall COMMAND testall)
