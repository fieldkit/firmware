if (TARGET Adafruit_TSL2591_Library)
  return()
endif()

add_external_arduino_library(Adafruit_TSL2591_Library)

find_package(Adafruit_Sensor)
target_link_libraries(Adafruit_TSL2591_Library Adafruit_Sensor)

find_package(Wire)
target_link_libraries(Adafruit_TSL2591_Library Wire)
