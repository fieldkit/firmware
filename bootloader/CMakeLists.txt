#
#
#
include(Arduino)

include(${CMAKE_CURRENT_SOURCE_DIR}/dependencies.cmake)

enable_arduino_toolchain()

enable_language(ASM)

enable_m4_board(fieldkit_core)

file(GLOB sources *.cpp *.c *.s *.h)

add_arduino_library(fkbl "${sources}")

target_compile_options(fkbl PRIVATE -Wall -Werror -Wno-main)

find_package(SeggerRTT)
target_compile_options(SeggerRTT PUBLIC -DSEGGER_RTT_BUFFER_SIZE_UP=4096)
target_link_libraries(fkbl SeggerRTT)

find_package(CortexLoading)
target_compile_options(CortexLoading PUBLIC -DFKB_EXTERNAL_NEED_PRINTF)
target_link_libraries(fkbl CortexLoading)

find_package(Asf4)
target_link_libraries(fkbl Asf4)

find_package(arduino-logging)
target_link_libraries(fkbl arduino-logging)

configure_firmware_linker_script(fkbl ${CMAKE_SOURCE_DIR}/boards/samd51/bootloader.ld)

add_arduino_bootloader(fkbl)
