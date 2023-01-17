#
#
#
include(Arduino)

include(ArduinoTools)

include(${CMAKE_SOURCE_DIR}/fk/dependencies.cmake)

enable_arduino_toolchain()

enable_language(ASM)

enable_m4_board(${FIELDKIT_BOARD})

file(GLOB sources *.cpp *.c *.s *.h)

add_arduino_library(fkbl "${sources}")

if(${FIELDKIT_BOARD} STREQUAL "fieldkit_underwater")
    target_compile_options(fkbl PRIVATE -DFK_UNDERWATER=1)
endif()

target_compile_options(fkbl PRIVATE -Wall -Werror -Wno-main)

find_package(SeggerRTT)
target_compile_options(SeggerRTT PUBLIC -DSEGGER_RTT_BUFFER_SIZE_UP=4096)
target_link_libraries(fkbl SeggerRTT)

find_package(CortexLoading)
target_compile_options(CortexLoading PUBLIC -DFKB_EXTERNAL_NEED_PRINTF)
target_link_libraries(fkbl CortexLoading)

find_package(Samd51Common)
target_link_libraries(fkbl Samd51Common)

find_package(Asf4)
target_link_libraries(fkbl Asf4)

find_package(arduino-logging)
target_link_libraries(fkbl arduino-logging)

target_include_directories(fkbl PUBLIC ${CMAKE_SOURCE_DIR}/fk/common)

target_compile_options(fkbl PRIVATE -DFK_VERSION=${FK_VERSION})

configure_firmware_linker_script(fkbl ${CMAKE_SOURCE_DIR}/boards/samd51/bootloader.ld)

add_arduino_bootloader(fkbl)

enable_signed_bin_firmware(fkbl)
