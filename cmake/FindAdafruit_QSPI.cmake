if (TARGET Adafruit_QSPI)
  return()
endif()

set(Adafruit_QSPI_RECURSE True)

add_external_arduino_library(Adafruit_QSPI)

find_package(SPI)
target_link_libraries(Adafruit_QSPI SPI)

find_package(Adafruit_SPIFlash)
target_link_libraries(Adafruit_QSPI Adafruit_SPIFlash)
