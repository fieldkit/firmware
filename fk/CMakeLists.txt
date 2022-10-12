#
#
#
find_program(CCACHE_PROGRAM ccache)

if(CCACHE_PROGRAM)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

include(Arduino)

include(ArduinoTools)

include(${CMAKE_SOURCE_DIR}/fk/dependencies.cmake)

function(fk_add_builtin_module name)
  set(module_dir ${CMAKE_SOURCE_DIR}/modules/${name})
  set(module_firmware_dir ${module_dir}/main)
  set(target_name fk-module-${name})

  file(GLOB_RECURSE sources ${module_firmware_dir}/*.cpp ${module_firmware_dir}/*.c ${module_firmware_dir}/*.h)

  add_arduino_library(${target_name} "${sources}")
  set_target_properties(${target_name} PROPERTIES LINK_WHOLE_LIBRARY YES)
  target_include_directories(${target_name} PUBLIC ${CMAKE_SOURCE_DIR}/fk ${CMAKE_SOURCE_DIR}/third-party/nonstd ${CMAKE_SOURCE_DIR}/third-party/tl)
  target_link_libraries(fk-bundled ${target_name})
  target_compile_options(${target_name} PRIVATE -Wall -Werror -DFK_HARDWARE_FULL)

  find_package(arduino-logging)
  target_link_libraries(${target_name} arduino-logging)

  find_package(SeggerRTT)
  target_link_libraries(${target_name} SeggerRTT)

  find_package(Asf4)
  target_link_libraries(${target_name} Asf4)

  find_package(Samd51Common)
  target_link_libraries(${target_name} Samd51Common)

  # NOTE: For DateTime
  find_package(lwcron)
  target_link_libraries(${target_name} lwcron)

  # TODO: REMOVE
  find_package(nanopb)
  target_link_libraries(${target_name} nanopb)

  # TODO: REMOVE
  find_package(HttpParser)
  target_link_libraries(${target_name} HttpParser)

  # TODO: REMOVE
  find_package(Protocols)
  target_link_libraries(${target_name} app-protocol)
  target_link_libraries(${target_name} data-protocol)
  target_link_libraries(${target_name} atlas-protocol)
endfunction()

if(${FK_OVERSIZED})
  set(ARDUINO_PROJECT_FLAGS "${ARDUINO_PROJECT_FLAGS} -finstrument-functions -finstrument-functions-exclude-file-list=arduino-1.8.3,arduinog-logging,arduino-osh -finstrument-functions-exclude-function-list=alog,vlog,log_raw,logerror,loginfo,logdebug,logtrace,SEGGER_RTT,os_,fk_uptime,__get,dbg_")
endif()

set(ARDUINO_PROJECT_LD_FLAGS "-Wl,-wrap,malloc -Wl,-wrap,free")

enable_arduino_toolchain()

enable_language(ASM)

enable_m4_board(fieldkit_underwater)

file(GLOB_RECURSE sources *.cpp *.c *.h *.s)

add_arduino_library(fk-bundled "${sources}")

target_include_directories(fk-bundled PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/third-party/nonstd ${CMAKE_SOURCE_DIR}/third-party/tl)

target_compile_options(fk-bundled PRIVATE -Wall -Werror -DFK_HARDWARE_FULL)

find_package(SeggerRTT)
target_link_libraries(fk-bundled SeggerRTT)
target_compile_options(SeggerRTT PUBLIC -DSEGGER_RTT_BUFFER_SIZE_UP=4096)

find_package(CortexLoading)
target_link_libraries(fk-bundled CortexLoading)

find_package(ConservifyOS)
target_compile_options(ConservifyOS PUBLIC -DOS_CONFIG_HOOKS_CUSTOM)
target_link_libraries(fk-bundled ConservifyOS)

find_package(Asf4)
target_link_libraries(fk-bundled Asf4)

find_package(Dhara)
target_link_libraries(fk-bundled Dhara)

find_package(Samd51Common)
target_link_libraries(fk-bundled Samd51Common)

find_package(SleepyDog)
target_link_libraries(fk-bundled SleepyDog)

find_package(WiFi101)
target_link_libraries(fk-bundled WiFi101)

find_package(u8g2)
target_link_libraries(fk-bundled u8g2)

find_package(lwcron)
target_link_libraries(fk-bundled lwcron)

find_package(HttpParser)
target_link_libraries(fk-bundled HttpParser)

find_package(ArduinoMDNS)
target_link_libraries(fk-bundled ArduinoMDNS)

find_package(arduino-logging)
target_link_libraries(fk-bundled arduino-logging)

find_package(Adafruit_SPIFlash)
target_link_libraries(fk-bundled Adafruit_SPIFlash)

find_package(Protocols)
target_link_libraries(fk-bundled app-protocol)
target_link_libraries(fk-bundled data-protocol)

find_package(TinyGPS)
target_link_libraries(fk-bundled TinyGPS)

find_package(Phylum)
target_link_libraries(fk-bundled phylum)

find_package(QrCode)
target_link_libraries(fk-bundled QrCode)

find_package(ArduinoBase64)
target_link_libraries(fk-bundled arduino-base64)

find_package(TheThingsNetwork)
target_link_libraries(fk-bundled TheThingsNetwork)

fk_add_builtin_module(random)
fk_add_builtin_module(diagnostics)
fk_add_builtin_module(weather)
fk_add_builtin_module(distance)
fk_add_builtin_module(atlas)
fk_add_builtin_module(water)

if(${FK_TARGET_QSPI_MEMORY})
  configure_firmware_linker_script(fk-bundled ${CMAKE_SOURCE_DIR}/boards/samd51/qspi.ld)
  target_compile_options(fk-bundled PRIVATE -DFK_TARGET_QSPI_MEMORY)
else()
  configure_firmware_linker_script(fk-bundled ${CMAKE_SOURCE_DIR}/boards/samd51/main.ld)
endif()

if (${FK_PHYLUM})
  target_compile_options(fk-bundled PRIVATE -DFK_VERSION=${FK_VERSION} -DFK_PHYLUM=1)
  target_compile_options(fk-bundled PRIVATE -DFK_PHYLUM_AMPLIFICATION=${FK_PHYLUM_AMPLIFICATION})
  target_compile_options(fk-bundled PRIVATE -DFK_DEBUG_LOGGING_SD_DISABLED=1)
  target_compile_options(fk-bundled PRIVATE -DFK_UNDERWATER=1)
else()
  target_compile_options(fk-bundled PRIVATE -DFK_VERSION=${FK_VERSION})
  target_compile_options(fk-bundled PRIVATE -DFK_DEBUG_LOGGING_SD_DISABLED=1)
  target_compile_options(fk-bundled PRIVATE -DFK_UNDERWATER=1)
endif()

add_arduino_firmware(fk-bundled)

enable_fkb_firmware(fk-bundled)

configure_file(version.txt.in ${CMAKE_BINARY_DIR}/version.txt)

add_custom_target(package COMMAND ${CMAKE_SOURCE_DIR}/tools/package.sh ${CMAKE_BINARY_DIR}/../ ${FK_VERSION})
