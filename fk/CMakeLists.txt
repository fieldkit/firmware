#
#
#
include(Arduino)

enable_arduino_toolchain()

enable_m4_board(fieldkit_core)

file(GLOB sources *.cpp *.c *.h)

add_arduino_library(fk-bundled "${sources}")

target_compile_options(fk-bundled PRIVATE -Wall -Werror)

find_package(SeggerRTT)
target_link_libraries(fk-bundled SeggerRTT)

find_package(ConservifyOS)
target_link_libraries(fk-bundled ConservifyOS)

add_arduino_firmware(fk-bundled)
