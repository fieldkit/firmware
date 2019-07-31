#
#
#
find_program(CCACHE_PROGRAM ccache)

if(CCACHE_PROGRAM)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

include(Arduino)

include(ArduinoTools)

include(${CMAKE_SOURCE_DIR}/libraries/dependencies.cmake)

function(fk_add_builtin_module name)
  set(module_dir ${CMAKE_SOURCE_DIR}/modules/${name})
  set(module_firmware_dir ${module_dir}/main)
  set(target_name fk-module-${name})

  file(GLOB_RECURSE sources ${module_firmware_dir}/*.cpp ${module_firmware_dir}/*.c ${module_firmware_dir}/*.h)

  add_arduino_library(${target_name} "${sources}")
  set_target_properties(${target_name} PROPERTIES LINK_WHOLE_LIBRARY YES)
  target_include_directories(${target_name} PUBLIC ${CMAKE_SOURCE_DIR}/fk)
  target_link_libraries(fk-bundled ${target_name})

  find_package(arduino-logging)
  target_link_libraries(${target_name} arduino-logging)

  find_package(SeggerRTT)
  target_link_libraries(${target_name} SeggerRTT)

  find_package(Asf4)
  target_link_libraries(${target_name} Asf4)
endfunction()

enable_arduino_toolchain()

enable_language(ASM)

enable_m4_board(fieldkit_core)

file(GLOB_RECURSE sources *.cpp *.c *.h)

add_arduino_library(fk-bundled "${sources}")

target_include_directories(fk-bundled PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(fk-bundled PRIVATE -Wall -Werror -DFK_HARDWARE_FULL)

find_package(SeggerRTT)
target_link_libraries(fk-bundled SeggerRTT)

find_package(CortexLoading)
target_link_libraries(fk-bundled CortexLoading)

find_package(ConservifyOS)
target_link_libraries(fk-bundled ConservifyOS)

find_package(Asf4)
target_link_libraries(fk-bundled Asf4)

find_package(WiFi101)
target_link_libraries(fk-bundled WiFi101)

find_package(u8g2)
target_link_libraries(fk-bundled u8g2)

find_package(lwcron)
target_link_libraries(fk-bundled lwcron)

find_package(lwstreams)
target_link_libraries(fk-bundled lwstreams)

find_package(HttpParser)
target_link_libraries(fk-bundled HttpParser)

find_package(ArduinoMDNS)
target_link_libraries(fk-bundled ArduinoMDNS)

find_package(arduino-logging)
target_link_libraries(fk-bundled arduino-logging)

find_package(Adafruit_QSPI)
target_link_libraries(fk-bundled Adafruit_QSPI)

find_package(Protocols)
target_link_libraries(fk-bundled app-protocol)
target_link_libraries(fk-bundled data-protocol)

find_package(TinyGPS)
target_link_libraries(fk-bundled TinyGPS)

find_package(Phylum)
target_link_libraries(fk-bundled phylum)

fk_add_builtin_module(weather)
fk_add_builtin_module(water)
fk_add_builtin_module(random)

configure_firmware_linker_script(fk-bundled ${CMAKE_SOURCE_DIR}/boards/samd51/main.ld)

add_arduino_firmware(fk-bundled)

enable_fkb_firmware(fk-bundled)
