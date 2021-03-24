if(TARGET SerialFlash)
  return()
endif()

add_external_arduino_library(SerialFlash)

find_package(SPI)

target_link_libraries(SerialFlash SPI)
