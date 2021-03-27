include(ArduinoTools)

file(GLOB sources ../segger/*.c ../segger/*.h shim.c)
add_library(shim STATIC ${sources})

if(TARGET_M0)
  set(target_mcu "cortex-m0plus")
  set(target_fcpu "48000000l")
  set(target_board_flags -DF_CPU=${target_fcpu} -D__SAMD21__ -D__SAMD21G18A__ -DARM_MATH_CM0PLUS)
  set(target_c_flags -Os -s -ffunction-sections -fdata-sections -std=c99 -nostdlib --param max-inline-insns-single=500 -MMD -mcpu=${target_mcu} -mthumb ${target_board_flags} -fstack-usage) # -fno-threadsafe-statics -fno-rtti -fno-exceptions)
  set(linker_script ${CMAKE_CURRENT_SOURCE_DIR}/linking/samd21x18.ld)
  set(target_board_libraries -lm -larm_cortexM0l_math)
endif()

if(TARGET_M4)
  set(target_mcu "cortex-m4")
  set(target_fcpu "120000000L")
  set(target_board_flags -DF_CPU=${target_fcpu} -D__SAMD51__ -D__SAMD51P20A__ -D__FPU_PRESENT -DARM_MATH_CM4 -mfloat-abi=hard -mfpu=fpv4-sp-d16)
  set(target_c_flags -Os -s -ffunction-sections -fdata-sections -std=c99 -nostdlib --param max-inline-insns-single=500 -MMD -mcpu=${target_mcu} -mthumb ${target_board_flags} -fstack-usage) # -fno-threadsafe-statics -fno-rtti -fno-exceptions)
  set(linker_script ${CMAKE_CURRENT_SOURCE_DIR}/linking/samd51x20.ld)
  set(target_board_libraries -larm_cortexM4lf_math -mfloat-abi=hard -mfpu=fpv4-sp-d16)
endif()

target_compile_options(shim PUBLIC ${target_c_flags})

target_include_directories(shim PUBLIC ../include ../segger)

target_include_directories(shim PRIVATE ${ARDUINO_CMSIS_INCLUDE_DIRECTORY} ${ARDUINO_DEVICE_DIRECTORY})

add_custom_target(shim.elf)

add_dependencies(shim.elf shim)

add_custom_command(TARGET shim.elf POST_BUILD
    COMMAND ${CMAKE_C_COMPILER} -Os -save-temps -T${linker_script} -mcpu=${target_mcu} -mthumb
    --specs=nano.specs --specs=nosys.specs -Wl,--check-sections
    ${target_board_libraries}
    -Wl,--unresolved-symbols=report-all -Wl,--warn-common -Wl,--warn-section-align
    -o ${CMAKE_CURRENT_BINARY_DIR}/shim.elf
    -L${ARDUINO_CMSIS_DIRECTORY}/Lib/GCC/
    -L${CMAKE_CURRENT_SOURCE_DIR}/linking
    ${CMAKE_CURRENT_BINARY_DIR}/libshim.a
  )

set(target_name shim)

add_custom_target(${target_name}.bin)

add_dependencies(${target_name}.bin ${target_name}.elf)

add_custom_command(TARGET ${target_name}.bin POST_BUILD COMMAND ${ARDUINO_OBJCOPY} -O binary
  ${CMAKE_CURRENT_BINARY_DIR}/${target_name}.elf
  ${CMAKE_CURRENT_BINARY_DIR}/${target_name}.bin)

add_custom_target(${target_name}_bin ALL DEPENDS ${target_name}.bin)
