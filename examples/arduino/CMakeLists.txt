include(Arduino)

enable_arduino_toolchain()

file(GLOB sources *.cpp ../../src/*.cpp)

add_arduino_library(alogging-example-arduino "${sources}")

target_include_directories(alogging-example-arduino PUBLIC ./ ../../src)

add_arduino_firmware(alogging-example-arduino)
