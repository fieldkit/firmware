include(ExternalProject)

set(DEPENDENCIES)
set(SUPER_CMAKE_ARGS)

list(APPEND DEPENDENCIES lib_nanopb)
ExternalProject_Add(lib_nanopb
  GIT_REPOSITORY https://github.com/nanopb/nanopb.git
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/nanopb
  CMAKE_ARGS ${EXTRA_CMAKE_ARGS}
  INSTALL_COMMAND ""
)
ExternalProject_Get_Property(lib_nanopb source_dir)
list(APPEND SUPER_CMAKE_ARGS -DNanopb_ROOT=${source_dir}/extra)

ExternalProject_Add(fk_data_protocol
  DEPENDS ${DEPENDENCIES}
  SOURCE_DIR ${PROJECT_SOURCE_DIR}
  CMAKE_ARGS -DUSE_SUPERBUILD=OFF ${SUPER_CMAKE_ARGS}
  INSTALL_COMMAND ""
  BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/fk_data_protocol)

