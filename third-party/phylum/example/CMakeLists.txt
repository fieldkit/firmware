set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_CXX_STANDARD 14)

find_program(CCACHE_PROGRAM ccache)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/third-party/arduino-logging/cmake ${CMAKE_SOURCE_DIR}/cmake)

if(CCACHE_PROGRAM)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

file(GLOB sources *.cpp *.h ${CMAKE_SOURCE_DIR}/src/*.cpp ${CMAKE_SOURCE_DIR}/src/*.h)

add_executable(example ${sources})

target_compile_options(example PRIVATE -Wall -Wextra)

target_include_directories(example PRIVATE ${CMAKE_SOURCE_DIR}/src)

find_package(ArduinoLogging)
target_link_libraries(example ArduinoLogging)

find_package(Dhara)
target_link_libraries(example Dhara)
