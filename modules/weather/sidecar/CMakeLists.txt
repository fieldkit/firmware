include(ArduinoTools)

include(${CMAKE_SOURCE_DIR}/fk/dependencies.cmake)

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

function(add_sidecar_target profile flags)
	set(target_mcu "cortex-m0plus")
	set(target_board_flags -D__SAMD09__ -D__SAMD09D14A__ -DTINY_PRINTF_FEATURES_SMALL -DSEGGER_RTT_BUFFER_SIZE_UP=512 ${flags})
	set(target_c_flags -g -Os -s -fdata-sections -ffunction-sections -nostdlib --param max-inline-insns-single=500 -MMD -mcpu=${target_mcu} -mthumb ${target_board_flags} -fstack-usage -std=c99)
	set(linker_script ${CMAKE_SOURCE_DIR}/boards/samd09/main.ld)
	set(target_board_libraries)


	set(target_name fk-weather-sidecar-${profile})
	set(elf_file ${CMAKE_CURRENT_BINARY_DIR}/${target_name}.elf)
	set(bin_file ${CMAKE_CURRENT_BINARY_DIR}/${target_name}.bin)
	set(map_file ${CMAKE_CURRENT_BINARY_DIR}/${target_name}.map)
	set(sym_file ${CMAKE_CURRENT_BINARY_DIR}/${target_name}.sym)
	set(elf_target ${target_name}.elf)
	set(bin_target ${target_name}.bin)
	set(sym_target ${target_name}.sym)

	file(GLOB fk_sources ${CMAKE_SOURCE_DIR}/fk/modules/*.c)
	file(GLOB sources *.s *.c)
	add_library(${target_name} STATIC ${sources} ${fk_sources})
	target_compile_options(${target_name} PRIVATE ${target_c_flags})
	target_include_directories(${target_name} PUBLIC ${CMAKE_SOURCE_DIR}/fk/modules/shared)
	target_include_directories(${target_name} PUBLIC ${CMAKE_SOURCE_DIR}/modules/weather/main)



	file(GLOB segger_sources ${segger_PATH}/*.c)
	add_library(SeggerRTT-${profile} STATIC ${segger_sources})
	target_compile_options(SeggerRTT-${profile} PRIVATE ${target_c_flags})
	target_include_directories(SeggerRTT-${profile} PUBLIC ${segger_PATH})
	target_link_libraries(${target_name} SeggerRTT-${profile})


	file(GLOB fk_shared_sources ${CMAKE_SOURCE_DIR}/fk/modules/shared/crc.c ${CMAKE_SOURCE_DIR}/fk/modules/shared/modules.c)
	add_library(fk-shared-${profile} STATIC ${fk_shared_sources})
	target_compile_options(fk-shared-${profile} PRIVATE ${target_c_flags})
	target_link_libraries(fk-shared-${profile} SeggerRTT)
	target_link_libraries(${target_name} fk-shared-${profile})


	file(GLOB asf_sources
		${asf4_PATH}/samd09/hal/src/*.c
		${asf4_PATH}/samd09/hpl/*/*.c
		${asf4_PATH}/samd09/hal/utils/src/*.c
		${asf4_PATH}/samd09/driver_init.c
		${asf4_PATH}/samd09/gcc/gcc/startup_samd09.c
	)
	add_library(Asf4-${profile} STATIC ${asf_sources})
	target_compile_options(Asf4-${profile} PRIVATE ${target_c_flags})
	target_include_directories(Asf4-${profile}
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
	target_link_libraries(${target_name} Asf4-${profile})


	add_custom_command(
		OUTPUT ${elf_file}
		DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/lib${target_name}.a
		COMMAND ${CMAKE_C_COMPILER} -Os -Wl,--gc-sections -save-temps -T${linker_script} -mcpu=${target_mcu} -mthumb
		--specs=nano.specs --specs=nosys.specs -Wl,--cref -Wl,--check-sections ${target_board_libraries}
		-Wl,--unresolved-symbols=report-all -Wl,--warn-common -Wl,--warn-section-align
		-Wl,-Map,${map_file} -o ${elf_file} -L${CMAKE_SOURCE_DIR}/boards/samd09
		${CMAKE_CURRENT_BINARY_DIR}/lib${target_name}.a
		${CMAKE_CURRENT_BINARY_DIR}/libfk-shared-${profile}.a
		${CMAKE_CURRENT_BINARY_DIR}/libAsf4-${profile}.a
		${CMAKE_CURRENT_BINARY_DIR}/libSeggerRTT-${profile}.a
	)

	add_custom_target(${elf_target} ALL DEPENDS ${elf_file})
	add_dependencies(${elf_target} ${target_name})

	add_custom_command(OUTPUT ${bin_file} DEPENDS ${elf_file} COMMAND ${ARDUINO_OBJCOPY} -O binary ${elf_file} ${bin_file})
	add_custom_target(${bin_target} ALL DEPENDS ${bin_file})
	add_dependencies(${bin_target} ${elf_target})

	add_custom_command(
		OUTPUT ${sym_file}
		DEPENDS ${elf_file}
		COMMAND ${ARDUINO_NM} -C -l --print-size --size-sort -r --radix=d ${elf_file} > ${sym_file})
	add_custom_target(${sym_target} ALL DEPENDS ${sym_file})
	add_dependencies(${sym_target} ${elf_target})
endfunction()

set(aggregated_flags -DFK_WEATHER_SIDECAR_SUBORDINATE=1 -DFK_ENABLE_SHT31_AND_MPL3115A2=1)
set(unmetered_flags -DFK_WEATHER_SIDECAR_SUBORDINATE=1 -DFK_WEATHER_UNMETERED=1)

add_sidecar_target("aggregated" "${aggregated_flags}")
add_sidecar_target("unmetered" "${unmetered_flags}")
