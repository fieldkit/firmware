if(TARGET RadioHead)
  return()
endif()

add_external_arduino_library(RadioHead)

find_package(SPI)

target_link_libraries(RadioHead SPI)
