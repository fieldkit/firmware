if (NOT DEFINED ARDUINO_IDE)
  foreach(path $ENV{HOME}/arduino-1.8.3 $ENV{HOME}/conservify/arduino-1.8.3
               $ENV{HOME}/workspace/arduino-1.8.3
               ${PROJECT_SOURCE_DIR}/../arduino-1.8.3 ${PROJECT_SOURCE_DIR}/../../arduino-1.8.3)
      if (EXISTS ${path})
        set(ARDUINO_IDE ${path})
        break()
    endif()
  endforeach()

  if (NOT DEFINED ARDUINO_IDE)
    message(FATAL_ERROR "Unable to find Arduino IDE")
  endif()
endif()

set(ARDUINO_PACKAGES_PATH "${ARDUINO_IDE}/packages")
set(ARDUINO_TOOLS_PATH "${ARDUINO_PACKAGES_PATH}/arduino/tools")
set(ARM_TOOLS "${ARDUINO_TOOLS_PATH}/arm-none-eabi-gcc/4.8.3-2014q1/bin")

set(ARDUINO_CMSIS_DIRECTORY "${ARDUINO_TOOLS_PATH}/CMSIS/4.5.0/CMSIS")
set(ARDUINO_CMSIS_INCLUDE_DIRECTORY "${ARDUINO_CMSIS_DIRECTORY}/Include/")
set(ARDUINO_DEVICE_DIRECTORY "${ARDUINO_TOOLS_PATH}/CMSIS-Atmel/1.2.0/CMSIS/Device/ATMEL")
set(ARDUINO_BOARD_CORE_ROOT "${ARDUINO_PACKAGES_PATH}/adafruit/hardware/samd/1.2.9")
set(ARDUINO_BOARD_CORE_LIBRARIES_PATH "${ARDUINO_BOARD_CORE_ROOT}/libraries")
set(ARDUINO_LIBRARIES_PATH "${ARDUINO_IDE}/libraries")
set(ARDUINO_CORE_DIRECTORY "${ARDUINO_BOARD_CORE_ROOT}/cores/arduino/")

set(ARDUINO_OBJCOPY "${ARM_TOOLS}/arm-none-eabi-objcopy")
set(ARDUINO_NM "${ARM_TOOLS}/arm-none-eabi-nm")
set(CXX_FILT "c++filt")
set(ARDUINO_PROJECT_FLAGS)
set(ARDUINO_PROJECT_LD_FLAGS)

# This used to be more buggy.
foreach(mp ${CMAKE_MODULE_PATH})
  if(EXISTS ${mp}/Arduino.cmake)
    set(module_path ${mp})
  endif()
endforeach()

function(enable_m0_target target_name target_board)
  set(target_mcu "cortex-m0plus")
  set(target_fcpu "48000000l")

  set(target_board_directory "${ARDUINO_BOARD_CORE_ROOT}/variants/${target_board}")

  set(target_board_flags "-DF_CPU=${target_fcpu} -DARDUINO=2491 -DARDUINO_M0PLUS=10605 -DARDUINO_SAMD_ZERO -DARM_MATH_CM0PLUS -DARDUINO_ARCH_SAMD -D__SAMD21__ -D__SAMD21G18A__ -DUSB_VID=0x2341 -DUSB_PID=0x804d -DUSBCON")
  set(target_c_flags "-g -Os -s -ffunction-sections -fdata-sections -nostdlib --param max-inline-insns-single=500 -MMD -mcpu=${target_mcu} -mthumb ${target_board_flags} ${ARDUINO_PROJECT_FLAGS}")
  set(target_cxx_flags "${target_c_flags} -fno-threadsafe-statics -fno-rtti -fno-exceptions")
  set(target_asm_flags "-g -x assembler-with-cpp -mcpu=${target_mcu} -mthumb ${target_board_flags}")

  set(target_board_ldflags "-mcpu=${target_mcu} -mthumb ${ARDUINO_PROJECT_LD_FLAGS}")
  set(target_board_libraries "-lm -larm_cortexM0l_math")
  set(target_includes ${ARDUINO_CMSIS_INCLUDE_DIRECTORY} ${ARDUINO_DEVICE_DIRECTORY} ${ARDUINO_CORE_DIRECTORY} ${target_board_directory})

  set(target_bootloader "${module_path}/linking/samd21x18_bootloader_large.ld")

  set_target_properties(${target_name} PROPERTIES
    target_board ${target_board}
    target_board_directory ${target_board_directory}
    target_board_flags ${target_board_flags}
    target_c_flags ${target_c_flags}
    target_cxx_flags ${target_cxx_flags}
    target_asm_flags ${target_asm_flags}
    target_board_ldflags ${target_board_ldflags}
    target_board_libraries ${target_board_libraries}
    target_includes "${target_includes}"
    target_bootloader ${target_bootloader}
  )

  message(STATUS "Enabled Cortex-M0 for ${target_name}")
endfunction()

function(enable_m4_target target_name target_board)
  set(target_mcu "cortex-m4")
  set(target_fcpu "120000000L")

  set(target_board_directory "${ARDUINO_BOARD_CORE_ROOT}/variants/${target_board}")

  if(FALSE)
    set(specific_board "-DARDUINO_FEATHER_M4 -DADAFRUIT_FEATHER_M4_EXPRESS -D__SAMD51J19A__")
  else()
    set(specific_board "-DARDUINO_GRAND_CENTRAL_M4 -DADAFRUIT_GRAND_CENTRAL_M4 -D__SAMD51P20A__")
  endif()

  if(${target_board} STREQUAL "fieldkit_core")
    set(target_board_flags "-DF_CPU=${target_fcpu} -DARDUINO=10803 -DARDUINO_ARCH_SAMD -D__SAMD51__ ${specific_board} -D__FPU_PRESENT -DARM_MATH_CM4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSB_VID=0x239A -DUSB_PID=0x8022 -DENABLE_CACHE -DMAX_SPI=24000000")
  else()
    set(target_board_flags "-DF_CPU=${target_fcpu} -DARDUINO=10803 -DARDUINO_ARCH_SAMD -D__SAMD51__ ${specific_board} -D__FPU_PRESENT -DARM_MATH_CM4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DUSB_VID=0x239A -DUSB_PID=0x8022 -DENABLE_CACHE -DMAX_SPI=24000000 -DUSBCON")
  endif()
  set(target_c_flags "-g -Os -s -ffunction-sections -fdata-sections -nostdlib --param max-inline-insns-single=500 -MMD -mcpu=${target_mcu} -mthumb ${target_board_flags} ${ARDUINO_PROJECT_FLAGS}")
  set(target_cxx_flags "${target_c_flags} -fno-threadsafe-statics -fno-rtti -fno-exceptions")
  set(target_asm_flags "-g -x assembler-with-cpp -mcpu=${target_mcu} -mthumb ${target_board_flags}")

  set(target_board_ldflags "-mcpu=${target_mcu} -mthumb ${ARDUINO_PROJECT_LD_FLAGS}")
  set(target_board_libraries "-larm_cortexM4lf_math -mfloat-abi=hard -mfpu=fpv4-sp-d16 -lstdc++")
  set(target_includes ${ARDUINO_CMSIS_INCLUDE_DIRECTORY} ${ARDUINO_DEVICE_DIRECTORY} ${ARDUINO_CORE_DIRECTORY} ${target_board_directory})

  set(target_bootloader "${module_path}/linking/samd51x19_bootloader_small.ld")

  set_target_properties(${target_name} PROPERTIES
    target_board ${target_board}
    target_board_directory ${target_board_directory}
    target_board_flags ${target_board_flags}
    target_c_flags ${target_c_flags}
    target_cxx_flags ${target_cxx_flags}
    target_asm_flags ${target_asm_flags}
    target_board_ldflags ${target_board_ldflags}
    target_board_libraries ${target_board_libraries}
    target_includes "${target_includes}"
    target_bootloader ${target_bootloader}
  )

  message(STATUS "Enabled Cortex-M4 for ${target_name} / ${target_board}")
endfunction()

function(enable_small_bootloader_target target_name)
  set_target_properties(${target_name} PROPERTIES target_bootloader "${module_path}/linking/samd21x18_bootloader_small.ld")

  message(STATUS "Linking for use with small bootloader.")
endfunction()

function(enable_large_bootloader_target target_name)
  set_target_properties(${target_name} PROPERTIES target_bootloader "${module_path}/linking/samd21x18_bootloader_large.ld")

  message(STATUS "Linking for use with large bootloader.")
endfunction()

function(enable_m0)
  message(STATUS "Enabled Cortex-M0")
  set(ARDUINO_BOARD_M0 "feather_m0" CACHE INTERNAL "" FORCE)
  set(ARDUINO_BOARD_M4 "" CACHE INTERNAL "" FORCE)
endfunction()

function(enable_m4_board board)
  message(STATUS "Enabled Cortex-M4 (board = ${board})")
  set(ARDUINO_BOARD_M4 "${board}" CACHE INTERNAL "" FORCE)
  set(ARDUINO_BOARD_M0 "" CACHE INTERNAL "" FORCE)
endfunction()

function(enable_m4)
  enable_m4_board("grand_central_m4")
endfunction()

function(enable_small_bootloader)
  message(STATUS "enable_small_bootloader() is deprecated")
endfunction()

function(enable_large_bootloader)
  message(STATUS "enable_large_bootloader() is deprecated")
endfunction()

# Not a huge fan of this. There doesn't seem to be a good way of setting C/C++
# flags separately for a target, though.
function(apply_compile_flags files new_c_flags new_cxx_flags new_asm_flags)
  foreach(file ${files})
    if(${file} MATCHES ".c$")
      set_source_files_properties(${file} PROPERTIES COMPILE_FLAGS "${new_c_flags}")
    endif()
    if(${file} MATCHES ".cpp$")
      set_source_files_properties(${file} PROPERTIES COMPILE_FLAGS "${new_cxx_flags}")
    endif()
    if(${file} MATCHES ".s$")
      set_source_files_properties(${file} PROPERTIES COMPILE_FLAGS "${new_asm_flags}")
    endif()
    if(${file} MATCHES ".S$")
      set_source_files_properties(${file} PROPERTIES COMPILE_FLAGS "${new_asm_flags}")
    endif()
  endforeach()
endfunction()

function(configure_compile_options target_name sources)
  get_target_property(target_c_flags ${target_name} target_c_flags)
  get_target_property(target_cxx_flags ${target_name} target_cxx_flags)
  get_target_property(target_asm_flags ${target_name} target_asm_flags)
  get_target_property(target_includes ${target_name} target_includes)

  target_include_directories(${target_name} PUBLIC "${target_includes}")
  set_target_properties(${target_name} PROPERTIES C_STANDARD 11)
  set_target_properties(${target_name} PROPERTIES CXX_STANDARD 11)
  apply_compile_flags("${sources}" ${target_c_flags} ${target_cxx_flags} ${target_asm_flags})
endfunction()

function(configure_arduino_core_target target_board)
  set(core_target_name "arduino-core-${target_board}")

  if(TARGET ${core_target_name})
    return()
  endif()

  set(target_board_directory "${ARDUINO_BOARD_CORE_ROOT}/variants/${target_board}")

  file(GLOB_RECURSE sources ${target_board_directory}/variant.cpp ${ARDUINO_CORE_DIRECTORY}/*.c ${ARDUINO_CORE_DIRECTORY}/*.cpp)

  add_library(${core_target_name} STATIC ${sources})

  configure_target_board(${core_target_name})

  configure_compile_options(${core_target_name} "${sources}")
endfunction()

# Call from the project.
function(enable_arduino_toolchain)
  set(CMAKE_C_COMPILER "${ARM_TOOLS}/arm-none-eabi-gcc" PARENT_SCOPE)
  set(CMAKE_CXX_COMPILER "${ARM_TOOLS}/arm-none-eabi-g++" PARENT_SCOPE)
  set(CMAKE_ASM_COMPILER "${ARM_TOOLS}/arm-none-eabi-gcc" PARENT_SCOPE)
  set(CMAKE_AR "${ARM_TOOLS}/arm-none-eabi-ar" PARENT_SCOPE)
  set(CMAKE_RANLIB "${ARM_TOOLS}/arm-none-eabi-ranlib" PARENT_SCOPE)

  enable_m0()
endfunction()

function(configure_target_board target_name)
  if(NOT "${ARDUINO_BOARD_M0}" STREQUAL "")
    enable_m0_target(${target_name} ${ARDUINO_BOARD_M0})
  endif()
  if(NOT "${ARDUINO_BOARD_M4}" STREQUAL "")
    enable_m4_target(${target_name} ${ARDUINO_BOARD_M4})
  endif()
endfunction()

function(configure_firmware_linker_script target_name script)
  set_target_properties(${target_name} PROPERTIES target_bootloader ${script})
endfunction()

function(configure_firmware_link target_name additional_libraries)
  get_target_property(libraries ${target_name} LINK_LIBRARIES)

  get_target_property(target_board_directory ${target_name} target_board_directory)
  get_target_property(target_board_flags ${target_name} target_board_flags)
  get_target_property(target_board_ldflags ${target_name} target_board_ldflags)
  get_target_property(target_board_libraries ${target_name} target_board_libraries)
  get_target_property(target_includes ${target_name} target_includes)
  get_target_property(target_bootloader ${target_name} target_bootloader)
  get_target_property(target_custom_ldflags ${target_name} target_custom_ldflags)

  set(dependencies)
  set(unique_libraries)
  if(NOT "${libraries}" STREQUAL "libraries-NOTFOUND")
    foreach(library ${libraries})
      get_property(libs TARGET ${library} PROPERTY INTERFACE_LINK_LIBRARIES)
      list(APPEND unique_libraries ${library})
      list(APPEND unique_libraries ${libs})
    endforeach()
    list(APPEND dependencies ${libraries})
  endif()
  list(APPEND dependencies  ${target_name})

  if("${unique_libraries}")
    list(REVERSE unique_libraries)
    list(REMOVE_DUPLICATES unique_libraries)
    list(REVERSE unique_libraries)
  endif()

  set(library_files)
  set(whole_library_files)

  foreach(library ${unique_libraries})
    get_target_property(library_dir ${library} BINARY_DIR)
    get_target_property(link_whole_library ${library} LINK_WHOLE_LIBRARY)

    if(${link_whole_library})
      list(APPEND whole_library_files ${library_dir}/lib${library}.a)
    elseif(${library} STREQUAL "ConservifyOS")
      list(APPEND whole_library_files ${library_dir}/lib${library}.a)
    else()
      list(APPEND library_files ${library_dir}/lib${library}.a)
    endif()
  endforeach()

  get_target_property(library_dir ${target_name} BINARY_DIR)
  list(APPEND whole_library_files ${library_dir}/lib${target_name}.a)

  set(elf_file ${CMAKE_CURRENT_BINARY_DIR}/${target_name}.elf)
  set(bin_file ${CMAKE_CURRENT_BINARY_DIR}/${target_name}.bin)
  set(sym_file ${CMAKE_CURRENT_BINARY_DIR}/${target_name}.sym)
  set(map_file ${CMAKE_CURRENT_BINARY_DIR}/${target_name}.map)
  set(elf_target ${target_name}.elf)
  set(bin_target ${target_name}.bin)
  set(sym_target ${target_name}.sym)

  get_filename_component(linker_script_include ${target_bootloader} DIRECTORY)
  set(linker_script ${target_bootloader})

  string(REPLACE " " ";" ldflags ${target_board_ldflags})
  string(REPLACE " " ";" board_libraries ${target_board_libraries})

  if(${target_custom_ldflags} STREQUAL "target_custom_ldflags-NOTFOUND")
	set(custom_ldflags "")
  else()
	string(REPLACE " " ";" custom_ldflags ${target_custom_ldflags})
  endif()

  add_custom_command(
    OUTPUT ${elf_file}
    DEPENDS ${dependencies}
    COMMAND ${CMAKE_C_COMPILER} -Os -Wl,--gc-sections -save-temps -T${linker_script}
    --specs=nano.specs --specs=nosys.specs -Wl,--cref -Wl,--check-sections ${ldflags}
    -Wl,--gc-sections -Wl,--unresolved-symbols=report-all -Wl,--warn-common -Wl,--warn-section-align -Wl,--emit-relocs
    -Wl,-Map,${map_file} ${custom_ldflags} -o ${elf_file}
    -L${ARDUINO_CMSIS_DIRECTORY}/Lib/GCC/ -L${linker_script_include}
    -Wl,--whole-archive ${whole_library_files} -Wl,--no-whole-archive
    ${library_files} ${additional_libraries} ${board_libraries}
  )
  add_custom_target(${elf_target} ALL DEPENDS ${elf_file})
  add_dependencies(${elf_target} ${target_name})

  add_custom_command(
    OUTPUT ${bin_file}
    DEPENDS ${elf_file}
    COMMAND ${ARDUINO_OBJCOPY} -O binary ${elf_file} ${bin_file}
  )
  add_custom_target(${bin_target} ALL DEPENDS ${bin_file})
  add_dependencies(${bin_target} ${elf_target})

  add_custom_command(
    OUTPUT ${sym_file}
    DEPENDS ${elf_file}
    COMMAND ${ARDUINO_NM} -C -l --print-size --size-sort -r --radix=d ${elf_file} | ${CXX_FILT} > ${sym_file})
  add_custom_target(${sym_target} ALL DEPENDS ${sym_file})
  add_dependencies(${sym_target} ${elf_target})

  set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES "${elf_file}" "${bin_file}" "${map_file}")
endfunction()

function(add_arduino_library target_name sources)
  add_library(${target_name} STATIC ${sources})
  configure_target_board(${target_name})
  configure_compile_options(${target_name} "${sources}")
endfunction()

function(add_arduino_firmware target_name)
  get_target_property(updated_sources ${target_name} SOURCES)
  get_target_property(target_board ${target_name} target_board)

  list(INSERT updated_sources 0 ${ARDUINO_CORE_DIRECTORY}/main.cpp)
  configure_compile_options(${target_name} "${updated_sources}")
  target_sources(${target_name} PUBLIC ${updated_sources})

  configure_arduino_core_target(${target_board})

  target_link_libraries(${target_name} arduino-core-${target_board})

  configure_firmware_link(${target_name} "")
endfunction()

function(add_arduino_bootloader target_name)
  get_target_property(target_board ${target_name} target_board)

  configure_arduino_core_target(${target_board})

  target_link_libraries(${target_name} arduino-core-${target_board})

  configure_firmware_link(${target_name} "")
endfunction()

function(add_external_arduino_library name)
  if(NOT DEFINED EXTERNAL_DEPENDENCIES)
    include(${CMAKE_CURRENT_SOURCE_DIR}/dependencies.cmake)
  endif()

  if (NOT EXISTS ${${name}_PATH})
    message(FATAL_ERROR "No ${name}_PATH")
  endif()

  set(path "${${name}_PATH}")
  set(recurse "${${name}_RECURSE}")

  set(source_paths)
  if (EXISTS "${path}/src")
    list(APPEND source_paths "${path}/src")
  endif()
  if (EXISTS "${path}/cppsrc")
    list(APPEND source_paths "${path}/cppsrc")
  endif()
  if (EXISTS "${path}/csrc")
    list(APPEND source_paths "${path}/csrc")
  endif()

  if (NOT source_paths)
    list(APPEND source_paths ${path})
  endif()

  message(STATUS "Library: ${name} (${source_paths})")

  if (NOT TARGET ${name})
    set(sources)

    foreach(source_path ${source_paths})
      if (recurse)
        file(GLOB_RECURSE path_sources ${source_path}/*.c ${source_path}/*.cpp ${source_path}/*.S)
      else()
        file(GLOB path_sources ${source_path}/*.c ${source_path}/*.cpp ${source_path}/*.S)
      endif()
      list(APPEND sources ${path_sources})
    endforeach()

    add_arduino_library(${name} "${sources}")

    target_include_directories(${name}
      PUBLIC ${source_paths}
      PRIVATE ${source_paths}
    )
  endif()
endfunction()

function(add_gitdeps)
  if(NOT DEFINED EXTERNAL_DEPENDENCIES)
    message(STATUS "Including: ${CMAKE_CURRENT_SOURCE_DIR}/dependencies.cmake")
    include(${CMAKE_CURRENT_SOURCE_DIR}/dependencies.cmake)
  endif()

  foreach(name ${EXTERNAL_DEPENDENCIES})
    add_external_arduino_library(${name})
  endforeach()
endfunction()
