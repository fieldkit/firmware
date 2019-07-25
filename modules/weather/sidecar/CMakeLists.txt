include(ArduinoTools)

include(dependencies.cmake)

set(asf4_PATH ${CMAKE_SOURCE_DIR}/third-party/asf4)


set(ARDUINO_PACKAGES_PATH "${ARDUINO_IDE}/packages")
set(ARDUINO_TOOLS_PATH "${ARDUINO_PACKAGES_PATH}/arduino/tools")
set(ARM_TOOLS "${ARDUINO_TOOLS_PATH}/arm-none-eabi-gcc/4.8.3-2014q1/bin")

set(ARDUINO_OBJCOPY "${ARM_TOOLS}/arm-none-eabi-objcopy")
set(ARDUINO_NM "${ARM_TOOLS}/arm-none-eabi-nm")

set(CMAKE_C_COMPILER ${ARM_TOOLS}/arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER ${ARM_TOOLS}/arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER ${ARM_TOOLS}/arm-none-eabi-gcc)

enable_language(ASM)

set(target_mcu "cortex-m0plus")
set(target_board_flags -D__SAMD09__ -D__SAMD09D14A__)
set(target_c_flags -g -Os -s -fdata-sections -ffunction-sections -nostdlib --param max-inline-insns-single=500 -MMD -mcpu=${target_mcu} -mthumb ${target_board_flags} -fstack-usage -std=c99)
set(linker_script ${CMAKE_SOURCE_DIR}/boards/samd09/main.ld)
set(target_board_libraries)


set(target_name fk-weather-sidecar)
set(elf_file ${CMAKE_CURRENT_BINARY_DIR}/${target_name}.elf)
set(bin_file ${CMAKE_CURRENT_BINARY_DIR}/${target_name}.bin)
set(map_file ${CMAKE_CURRENT_BINARY_DIR}/${target_name}.map)
set(elf_target ${target_name}.elf)
set(bin_target ${target_name}.bin)

file(GLOB fk_sources ${CMAKE_SOURCE_DIR}/fk/modules/*.c)
file(GLOB sources *.s *.c)
add_library(fk-weather-sidecar STATIC ${sources} ${fk_sources})
target_compile_options(fk-weather-sidecar PRIVATE ${target_c_flags})
target_include_directories(fk-weather-sidecar PUBLIC ${CMAKE_SOURCE_DIR}/fk)



file(GLOB segger_sources ${segger_PATH}/*.c)
add_library(SeggerRTT STATIC ${segger_sources})
target_compile_options(SeggerRTT PRIVATE ${target_c_flags})
target_include_directories(SeggerRTT PUBLIC ${segger_PATH})
target_link_libraries(fk-weather-sidecar SeggerRTT)



file(GLOB asf_sources
  ${asf4_PATH}/samd09/hal/src/*.c
  ${asf4_PATH}/samd09/hpl/*/*.c
  ${asf4_PATH}/samd09/hal/utils/src/*.c
  ${asf4_PATH}/samd09/atmel_start.c
  ${asf4_PATH}/samd09/driver_init.c
  ${asf4_PATH}/samd09/gcc/gcc/startup_samd09.c
)
add_library(Asf4 STATIC ${asf_sources})
target_compile_options(Asf4 PRIVATE ${target_c_flags})
target_include_directories(Asf4
  PUBLIC
  ${asf4_PATH}/samd09/config
  ${asf4_PATH}/samd09/include
  ${asf4_PATH}/samd09/hri
  ${asf4_PATH}/samd09/hal/include
  ${asf4_PATH}/samd09/hal/utils/include
  ${asf4_PATH}/samd09/hpl/core
  ${asf4_PATH}/samd09/hpl/gclk
  ${asf4_PATH}/samd09/hpl/wdt
  ${asf4_PATH}/samd09/hpl/port
  ${asf4_PATH}/samd09/hpl/tc
  ${asf4_PATH}/samd09/hpl/pm
  ${asf4_PATH}/samd09
  ${asf4_PATH}/samd09/CMSIS/Core/Include
)
target_link_libraries(fk-weather-sidecar Asf4)


add_custom_command(
  OUTPUT ${elf_file}
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/libfk-weather-sidecar.a
  COMMAND ${CMAKE_C_COMPILER} -Os -Wl,--gc-sections -save-temps -T${linker_script} -mcpu=${target_mcu} -mthumb
  --specs=nano.specs --specs=nosys.specs -Wl,--check-sections ${target_board_libraries}
  -Wl,--unresolved-symbols=report-all -Wl,--warn-common -Wl,--warn-section-align
  -Wl,-Map,${map_file} -o ${elf_file} -L${CMAKE_SOURCE_DIR}/boards/samd09
  ${CMAKE_CURRENT_BINARY_DIR}/libfk-weather-sidecar.a
  ${CMAKE_CURRENT_BINARY_DIR}/libAsf4.a
)

add_custom_target(${elf_target} ALL DEPENDS ${elf_file})
add_dependencies(${elf_target} ${target_name})

add_custom_command(OUTPUT ${bin_file} COMMAND ${ARDUINO_OBJCOPY} -O binary ${elf_file} ${bin_file})
add_custom_target(${bin_target} ALL DEPENDS ${bin_file})
add_dependencies(${bin_target} ${elf_target})
