if (TARGET ArduinoSound)
  return()
endif()

add_external_arduino_library(ArduinoSound)

find_package(I2S)
target_link_libraries(ArduinoSound I2S)

find_package(SD)
target_link_libraries(ArduinoSound SD)
