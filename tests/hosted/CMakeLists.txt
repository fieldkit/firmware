#
#
#

include(${CMAKE_SOURCE_DIR}/libraries/dependencies.cmake)

set(CMAKE_BUILD_TYPE DEBUG)

find_package(Threads REQUIRED)

include(ExternalProject)

set(GTEST_URL ${CMAKE_SOURCE_DIR}/third-party/googletest-1.8.0.zip)
set(GTEST_SHA256 SHA256=f3ed3b58511efd272eb074a3a6d6fb79d7c2e6a0e374323d1e6bcbcc1ef141bf)
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
  "IMPORTED_LOCATION" "${binary_dir}/googlemock/gtest/libgtest.a"
  "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
)

# Create a libgmock target to be used as a dependency by test programs
add_library(libgmock IMPORTED STATIC GLOBAL)
add_dependencies(libgmock gtest)
set_target_properties(libgmock PROPERTIES
  "IMPORTED_LOCATION" "${binary_dir}/googlemock/libgmock.a"
  "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
)

# I couldn't make it work with INTERFACE_INCLUDE_DIRECTORIES
include_directories("${source_dir}/googletest/include"
                    "${source_dir}/googlemock/include")

file(GLOB fk_sources ../../fk/*.cpp ../../fk/httpd/*.cpp)
list(FILTER fk_sources EXCLUDE REGEX ".*main.cpp$")
list(FILTER fk_sources EXCLUDE REGEX ".*platform.cpp$")

file(GLOB sources *.cpp ${fk_sources})

add_executable(testall ${sources})

find_package(lwstreams)
target_link_libraries(testall lwstreams)

find_package(HttpParser)
target_link_libraries(testall HttpParser)

target_include_directories(testall PRIVATE ../../fk)
target_compile_options(testall PUBLIC -Wall -pedantic)
target_link_libraries(testall libgtest libgmock)
add_test(NAME testall COMMAND testall)
