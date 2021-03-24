if (TARGET Adafruit_MPL3115A2_Library)
  return()
endif()

add_external_arduino_library(Adafruit_MPL3115A2_Library)

find_package(Adafruit_Sensor)

find_package(Wire)

target_link_libraries(Adafruit_MPL3115A2_Library Adafruit_Sensor)

target_link_libraries(Adafruit_MPL3115A2_Library Wire)
