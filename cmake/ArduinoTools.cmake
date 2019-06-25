#
#
#
function(enable_fkb_firmware target)
  get_target_property(binary_dir ${target} BINARY_DIR)

  add_custom_target(${target}-fkb.elf ALL
    ${CortexLoading_PATH}/tools/mkfirmware.py --elf ${binary_dir}/${target}.elf --fkb ${binary_dir}/${target}-fkb.elf
    DEPENDS ${target}.elf
  )

  add_custom_target(${target}-fkb.bin)

  add_dependencies(${target}-fkb.bin ${target}-fkb.elf)

  add_custom_command(TARGET ${target}-fkb.bin POST_BUILD COMMAND ${ARDUINO_OBJCOPY} -O binary
    ${CMAKE_CURRENT_BINARY_DIR}/${target}-fkb.elf
    ${CMAKE_CURRENT_BINARY_DIR}/${target}-fkb.bin)

  add_custom_target(${target}_fkb_bin ALL DEPENDS ${target}-fkb.bin)

  set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${target}.syms"
    "${CMAKE_CURRENT_BINARY_DIR}/${target}.s"
    "${CMAKE_CURRENT_BINARY_DIR}/${target}.elf"
    "${CMAKE_CURRENT_BINARY_DIR}/${target}.bin"
    "${CMAKE_CURRENT_BINARY_DIR}/${target}.fkb"
    "${CMAKE_CURRENT_BINARY_DIR}/${target}-fkb.bin"
    "${CMAKE_CURRENT_BINARY_DIR}/${target}.map")
endfunction()
