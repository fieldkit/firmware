#
#
#
if(TARGET ArduinoMDNS)
  return()
endif()

file(GLOB sources ${ArduinoMDNS_PATH}/*.cpp ${ArduinoMDNS_PATH}/utility/*.c)

add_arduino_library(ArduinoMDNS "${sources}")

target_include_directories(ArduinoMDNS
  PUBLIC ${ArduinoMDNS_PATH}
  PRIVATE ${ArduinoMDNS_PATH}
)
