#
#
#
include(Arduino)

enable_arduino_toolchain()

enable_language(ASM)

enable_m4_board(fieldkit_core)

file(GLOB sources *.cpp *.c *.s *.h)

add_arduino_library(fkbl "${sources}")

target_compile_options(fkbl PRIVATE -Wall -Werror -Wno-main)

find_package(SeggerRTT)
target_link_libraries(fkbl SeggerRTT)

find_package(CortexLoading)
target_link_libraries(fkbl CortexLoading)

find_package(arduino-logging)
target_link_libraries(fkbl arduino-logging)

configure_firmware_linker_script(fkbl ${CMAKE_SOURCE_DIR}/boards/samd51/bootloader.ld)

add_arduino_bootloader(fkbl)
