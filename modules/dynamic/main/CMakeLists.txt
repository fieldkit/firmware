include(${CMAKE_SOURCE_DIR}/fk/dependencies.cmake)

include(Arduino)

enable_fk_module()

enable_arduino_toolchain()

enable_language(ASM)

enable_m4_board(${FIELDKIT_BOARD})

file(GLOB common_sources ../*.cpp ../*.c)

file(GLOB module_sources *.c *.cpp)
add_arduino_library(fkdynamic "${common_sources};${module_sources}")
target_compile_options(fkdynamic PRIVATE -Wall -Werror)
add_fk_module(fkdynamic)
