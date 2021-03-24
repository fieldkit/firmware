if(TARGET Adafruit_Sensor)
  return()
endif()

add_external_arduino_library(Adafruit_Sensor)

find_package(Wire)

target_link_libraries(Adafruit_Sensor Wire)
