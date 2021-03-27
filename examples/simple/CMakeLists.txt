include(Arduino)

enable_arduino_toolchain()

file(GLOB sources *.cpp ../../src/*.cpp)

include_directories(../../src)

add_arduino_library(lwcron-example-simple "${sources}")

target_compile_options(lwcron-example-simple PRIVATE -Wall -Werror)

add_arduino_firmware(lwcron-example-simple)
