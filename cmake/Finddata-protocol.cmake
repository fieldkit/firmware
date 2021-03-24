if(TARGET data-protocol)
  return()
endif()

add_external_arduino_library(data-protocol)

find_package(nanopb)

target_link_libraries(data-protocol nanopb)
