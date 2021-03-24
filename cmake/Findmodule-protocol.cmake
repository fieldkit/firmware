if(TARGET module-protocol)
  return()
endif()

add_external_arduino_library(module-protocol)

find_package(nanopb)

target_link_libraries(module-protocol nanopb)
