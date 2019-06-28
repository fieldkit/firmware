#
#
#
if(TARGET ArduinoMDNS)
  return()
endif()

set(ArduinoMDNS_PATH ${CMAKE_SOURCE_DIR}/libraries/arduino-libraries/ArduinoMDNS)

file(GLOB sources ${ArduinoMDNS_PATH}/*.cpp ${ArduinoMDNS_PATH}/utility/*.c)

add_arduino_library(ArduinoMDNS "${sources}")

target_include_directories(ArduinoMDNS 
  PUBLIC ${ArduinoMDNS_PATH}
  PRIVATE ${ArduinoMDNS_PATH}
)
