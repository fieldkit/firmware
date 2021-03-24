if(TARGET app-protocol)
  return()
endif()

add_external_arduino_library(app-protocol)

find_package(nanopb)

target_link_libraries(app-protocol nanopb)
