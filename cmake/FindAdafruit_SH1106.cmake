if (TARGET Adafruit_SH1106)
  return()
endif()

add_external_arduino_library(Adafruit_SH1106)

find_package(SPI)
target_link_libraries(Adafruit_SH1106 SPI)

find_package(Wire)
target_link_libraries(Adafruit_SH1106 Wire)
