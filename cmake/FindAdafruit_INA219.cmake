if (TARGET Adafruit_INA219)
  return()
endif()

add_external_arduino_library(Adafruit_INA219)

find_package(Wire)
target_link_libraries(Adafruit_INA219 Wire)
