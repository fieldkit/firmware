#
#
#

if(TARGET_LINUX)

# GTest requires this.
find_package(Threads REQUIRED)

# Enable ExternalProject CMake module
include(ExternalProject)

# Download and install gtest
ExternalProject_Add(
    gtest
    URL https://github.com/google/googletest/archive/release-1.8.0.zip
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest
    INSTALL_COMMAND ""
)

# Get GTest source and binary directories from CMake project
ExternalProject_Get_Property(gtest source_dir binary_dir)

# Create a libgtest target to be used as a dependency by test programs
add_library(libgtest IMPORTED STATIC GLOBAL)
add_dependencies(libgtest gtest)

# Set libgtest properties
set_target_properties(libgtest PROPERTIES
  "IMPORTED_LOCATION" "${binary_dir}/googlemock/gtest/libgtest.a"
  "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
)

# Create a libgmock target to be used as a dependency by test programs
add_library(libgmock IMPORTED STATIC GLOBAL)
add_dependencies(libgmock gtest)

# Set libgmock properties
set_target_properties(libgmock PROPERTIES
  "IMPORTED_LOCATION" "${binary_dir}/googlemock/libgmock.a"
  "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
)

# I couldn't make it work with INTERFACE_INCLUDE_DIRECTORIES
include_directories("${source_dir}/googletest/include"
                    "${source_dir}/googlemock/include")

file(GLOB SRCS *.cpp ../../src/*.c ../../src/*.h)

add_executable(hostedtests ${SRCS})

target_include_directories(hostedtests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(hostedtests PUBLIC ${source_dir})
target_include_directories(hostedtests PUBLIC "../../src")

target_link_libraries(hostedtests libgtest libgmock)

set_target_properties(hostedtests PROPERTIES C_STANDARD 11)
set_target_properties(hostedtests PROPERTIES CXX_STANDARD 11)

add_test(NAME hostedtests COMMAND hostedtests)

endif()
