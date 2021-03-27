include(ArduinoTools)

file(GLOB sources ../segger/*.c ../segger/*.h ../src/*.s ../src/*.c *.c)
add_library(bootloader STATIC ${sources})

if(TARGET_M0)
  set(target_mcu "cortex-m0plus")
  set(target_fcpu "48000000l")
  set(target_board_flags -DF_CPU=${target_fcpu} -D__SAMD21__ -D__SAMD21G18A__ -DFKB_EXTERNAL_NEED_PRINTF -DARM_MATH_CM0PLUS)
  set(target_c_flags -g -Os -s -fdata-sections -nostdlib --param max-inline-insns-single=500 -MMD -mcpu=${target_mcu} -mthumb ${target_board_flags} -fstack-usage -nostdlib -std=c99)
  set(linker_script ${CMAKE_CURRENT_SOURCE_DIR}/linking/samd21x18.ld)
  set(target_board_libraries -lm -larm_cortexM0l_math)
  set(CMAKE_ASM_FLAGS "-g -x assembler-with-cpp -mcpu=${target_mcu} -mthumb -D__SAMD21__")
endif()

if(TARGET_M4)
  set(target_mcu "cortex-m4")
  set(target_fcpu "120000000L")
  set(target_board_flags -DF_CPU=${target_fcpu} -D__SAMD51__ -D__SAMD51P20A__ -DFKB_EXTERNAL_NEED_PRINTF -D__FPU_PRESENT -DARM_MATH_CM4 -mfloat-abi=hard -mfpu=fpv4-sp-d16)
  set(target_c_flags -g -Os -s -fdata-sections -nostdlib --param max-inline-insns-single=500 -MMD -mcpu=${target_mcu} -mthumb ${target_board_flags} -fstack-usage -nostdlib -std=c99)
  set(linker_script ${CMAKE_CURRENT_SOURCE_DIR}/linking/samd51x20.ld)
  set(target_board_libraries -larm_cortexM4lf_math -mfloat-abi=hard -mfpu=fpv4-sp-d16)
  set(CMAKE_ASM_FLAGS "-g -x assembler-with-cpp -mcpu=${target_mcu} -mthumb -D__SAMD51__")
endif()

target_compile_options(bootloader PRIVATE ${target_c_flags})

target_include_directories(bootloader PUBLIC ../include ../segger)

target_include_directories(bootloader PRIVATE ${ARDUINO_CMSIS_INCLUDE_DIRECTORY} ${ARDUINO_DEVICE_DIRECTORY})

add_custom_target(bootloader.elf)

add_dependencies(bootloader.elf bootloader)

add_custom_command(TARGET bootloader.elf POST_BUILD
    COMMAND ${CMAKE_C_COMPILER} -Os -Wl,--gc-sections -save-temps -T${linker_script} -mcpu=${target_mcu} -mthumb
    --specs=nano.specs --specs=nosys.specs -Wl,--check-sections
    ${target_board_libraries}
    -Wl,--unresolved-symbols=report-all -Wl,--warn-common -Wl,--warn-section-align
    -o ${CMAKE_CURRENT_BINARY_DIR}/bootloader.elf
    -L${ARDUINO_CMSIS_DIRECTORY}/Lib/GCC/
    -L${CMAKE_CURRENT_SOURCE_DIR}/linking
    ${CMAKE_CURRENT_BINARY_DIR}/libbootloader.a
  )

set(target_name bootloader)

add_custom_target(${target_name}.bin)

add_dependencies(${target_name}.bin ${target_name}.elf)

add_custom_command(TARGET ${target_name}.bin POST_BUILD COMMAND ${ARDUINO_OBJCOPY} -O binary
  ${CMAKE_CURRENT_BINARY_DIR}/${target_name}.elf
  ${CMAKE_CURRENT_BINARY_DIR}/${target_name}.bin)

add_custom_target(${target_name}_bin ALL DEPENDS ${target_name}.bin)
