#
#
#
include(Arduino)
include(ArduinoTools)

enable_arduino_toolchain()

enable_language(ASM)

enable_m4_board(fieldkit_core)

include(${CMAKE_SOURCE_DIR}/libraries/dependencies.cmake)

file(GLOB sources *.cpp *.c *.h)

add_arduino_library(fk-bundled "${sources}")

target_compile_options(fk-bundled PRIVATE -Wall -Werror)

find_package(SeggerRTT)
target_link_libraries(fk-bundled SeggerRTT)

find_package(CortexLoading)
target_link_libraries(fk-bundled CortexLoading)

find_package(ConservifyOS)
target_link_libraries(fk-bundled ConservifyOS)

find_package(u8g2)
target_link_libraries(fk-bundled u8g2)

configure_firmware_linker_script(fk-bundled ${CMAKE_SOURCE_DIR}/boards/samd51/main.ld)

add_arduino_firmware(fk-bundled)

enable_fkb_firmware(fk-bundled)
