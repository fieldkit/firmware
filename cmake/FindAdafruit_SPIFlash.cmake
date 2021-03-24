if (TARGET Adafruit_SPIFlash)
  return()
endif()

set(Adafruit_SPIFlash_RECURSE YES)

add_external_arduino_library(Adafruit_SPIFlash)

target_include_directories(Adafruit_SPIFlash
  PUBLIC ${lwcron_PATH}/src ${lwcron_PATH}/src/qspi ${lwcron_PATH}/src/spi
  PRIVATE ${lwcron_PATH}/src ${lwcron_PATH}/src/qspi ${lwcron_PATH}/src/spi
)

find_package(SPI)
target_link_libraries(Adafruit_SPIFlash SPI)

find_package(SdFat)
target_link_libraries(Adafruit_SPIFlash SdFat)
