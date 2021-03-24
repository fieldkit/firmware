if(TARGET SPIMemory)
  return()
endif()

add_external_arduino_library(SPIMemory)

find_package(SPI)

target_link_libraries(SPIMemory SPI)
