if(TARGET u8glib)
  return()
endif()

add_external_arduino_library(u8glib)

find_package(SPI)
target_link_libraries(u8glib SPI)

find_package(Wire)
target_link_libraries(u8glib Wire)
