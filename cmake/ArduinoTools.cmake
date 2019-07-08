#
#
#
function(enable_fkb_firmware target)
  get_target_property(binary_dir ${target} BINARY_DIR)

  set(elf_target ${target}.elf)
  set(fkb_elf_target ${target}-fkb.elf)
  set(fkb_bin_target ${target}-fkb.bin)

  set(elf_file ${binary_dir}/${target}.elf)
  set(bin_file ${binary_dir}/${target}.bin)
  set(fkb_elf_file ${binary_dir}/${target}-fkb.elf)
  set(fkb_bin_file ${binary_dir}/${target}-fkb.bin)

  add_custom_command(
    OUTPUT ${fkb_elf_file}
    DEPENDS ${elf_file}
    COMMAND ${loading_PATH}/tools/mkfirmware.py --elf ${elf_file} --fkb ${fkb_elf_file}
    WORKING_DIRECTORY ${binary_dir}
    )

  add_custom_target(${fkb_elf_target} ALL DEPENDS ${elf_file})

  set_target_properties(${fkb_elf_target} PROPERTIES OUTPUT_NAME ${fkb_elf_file})

  add_custom_command(
    OUTPUT ${fkb_bin_file}
    DEPENDS ${fkb_elf_file}
    COMMAND ${ARDUINO_OBJCOPY} -O binary ${fkb_elf_file} ${fkb_bin_file}
    WORKING_DIRECTORY ${binary_dir}
    )

  add_custom_target(${fkb_bin_target} ALL DEPENDS ${fkb_bin_file})

  set_target_properties(${fkb_bin_target} PROPERTIES OUTPUT_NAME ${fkb_bin_file})

  set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${target}.syms"
    "${CMAKE_CURRENT_BINARY_DIR}/${target}.s"
    "${CMAKE_CURRENT_BINARY_DIR}/${target}.elf"
    "${CMAKE_CURRENT_BINARY_DIR}/${target}.bin"
    "${CMAKE_CURRENT_BINARY_DIR}/${target}.fkb"
    "${CMAKE_CURRENT_BINARY_DIR}/${target}-fkb.bin"
    "${CMAKE_CURRENT_BINARY_DIR}/${target}.map")
endfunction()
