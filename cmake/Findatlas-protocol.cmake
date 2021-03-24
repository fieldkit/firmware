if(TARGET atlas-protocol)
  return()
endif()

add_external_arduino_library(atlas-protocol)

find_package(nanopb)

target_link_libraries(atlas-protocol nanopb)
