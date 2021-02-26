#
#
#
function(enable_signed_bin_firmware target)
  get_target_property(binary_dir ${target} BINARY_DIR)

  set(elf_target ${target}.elf)
  set(fkb_bin_target ${target}-fkb.bin)

  set(elf_file ${binary_dir}/${target}.elf)
  set(fkb_bin_file ${binary_dir}/${target}-fkb.bin)

  add_custom_command(
    OUTPUT ${fkb_bin_file}
    DEPENDS ${elf_file}
    COMMAND ${loading_PATH}/tools/mkfirmware.py --elf ${elf_file} --bin ${fkb_bin_file}
    WORKING_DIRECTORY ${binary_dir}
    )

  add_custom_target(${fkb_bin_target} ALL DEPENDS ${fkb_bin_file})
  add_dependencies(${fkb_bin_target} ${elf_target})

  set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${target}.syms"
    "${CMAKE_CURRENT_BINARY_DIR}/${target}.s"
    "${CMAKE_CURRENT_BINARY_DIR}/${target}.elf"
    "${CMAKE_CURRENT_BINARY_DIR}/${target}.bin"
    "${CMAKE_CURRENT_BINARY_DIR}/${target}.map")
endfunction()

function(enable_fkb_firmware target)
  get_target_property(binary_dir ${target} BINARY_DIR)

  set(elf_target ${target}.elf)
  set(bin_target ${target}.bin)
  set(fkb_elf_target ${target}-fkb.elf)
  set(fkb_bin_target ${target}-fkb.bin)

  set(elf_file ${binary_dir}/${target}.elf)
  set(bin_file ${binary_dir}/${target}.bin)
  set(fkb_elf_file ${binary_dir}/${target}-fkb.elf)
  set(fkb_bin_file ${binary_dir}/${target}-fkb.bin)

  add_custom_command(
    OUTPUT ${fkb_elf_file}
    DEPENDS ${elf_file}
    COMMAND ${loading_PATH}/tools/mkfirmware.py --elf ${elf_file} --fkb ${fkb_elf_file} --bin ${fkb_bin_file}
    WORKING_DIRECTORY ${binary_dir}
    )

  add_custom_target(${fkb_elf_target} ALL DEPENDS ${fkb_elf_file})
  add_dependencies(${fkb_elf_target} ${elf_target})

  set_target_properties(${fkb_elf_target} PROPERTIES OUTPUT_NAME ${fkb_elf_file})

  set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${target}.syms"
    "${CMAKE_CURRENT_BINARY_DIR}/${target}.s"
    "${CMAKE_CURRENT_BINARY_DIR}/${target}.elf"
    "${CMAKE_CURRENT_BINARY_DIR}/${target}.bin"
    "${CMAKE_CURRENT_BINARY_DIR}/${target}.fkb"
    "${CMAKE_CURRENT_BINARY_DIR}/${target}-fkb.bin"
    "${CMAKE_CURRENT_BINARY_DIR}/${target}.map")
endfunction()

function(enable_samd09_firmware target)
endfunction()

function(enable_fk_module)
  # TODO Why isn't -fno-plt implemented?!
  set(ARDUINO_PROJECT_FLAGS "-fPIC -fno-inline -mpic-register=r9 -msingle-pic-base -mno-pic-data-is-text-relative" PARENT_SCOPE)
endfunction()

function(add_fk_module target_name)
  get_target_property(target_board ${target_name} target_board)

  configure_arduino_core_target(${target_board})

  set_target_properties(${target_name} PROPERTIES
    target_custom_ldflags "-nostartfiles -nodefaultlibs -nostdlib -Wl,--unresolved-symbols=ignore-in-object-files -shared"
  )

  find_package(CortexLoading)
  target_link_libraries(${target_name} CortexLoading)

  # find_package(ConservifyOS)
  # target_link_libraries(${target_name} ConservifyOS)

  configure_firmware_linker_script(${target_name} ${CMAKE_SOURCE_DIR}/boards/samd51/module.ld)

  configure_firmware_link(${target_name} "")
endfunction()


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
