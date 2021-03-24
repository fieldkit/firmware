if (TARGET Adafruit_BNO055)
  return()
endif()

add_external_arduino_library(Adafruit_BNO055)

find_package(Adafruit_Sensor)
target_link_libraries(Adafruit_BNO055 Adafruit_Sensor)

find_package(Wire)
target_link_libraries(Adafruit_BNO055 Wire)
