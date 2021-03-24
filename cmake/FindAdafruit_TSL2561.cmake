if (TARGET Adafruit_TSL2561)
  return()
endif()

add_external_arduino_library(Adafruit_TSL2561)

find_package(Adafruit_Sensor)
target_link_libraries(Adafruit_TSL2561 Adafruit_Sensor)

find_package(Wire)
target_link_libraries(Adafruit_TSL2561 Wire)
