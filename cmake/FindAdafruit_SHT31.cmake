if (TARGET Adafruit_SHT31)
  return()
endif()

add_external_arduino_library(Adafruit_SHT31)

find_package(Adafruit_Sensor)
target_link_libraries(Adafruit_SHT31 Adafruit_Sensor)

find_package(Wire)
target_link_libraries(Adafruit_SHT31 Wire)
