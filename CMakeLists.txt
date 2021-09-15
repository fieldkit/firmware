cmake_minimum_required(VERSION 3.2)

option(USE_SUPERBUILD "Whether or not a superbuild should be invoked" ON)

if(USE_SUPERBUILD)
  project(SUPERBUILD NONE)
  include(cmake/SuperBuild.cmake)
  return()
else()
  list(APPEND CMAKE_MODULE_PATH "${Nanopb_ROOT}")

  project(fk_data_protocol)
endif()

add_subdirectory(src)
