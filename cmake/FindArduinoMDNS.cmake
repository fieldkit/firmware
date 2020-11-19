#
#
#
if(TARGET ArduinoMDNS)
  return()
endif()

if(${TARGET_ARCH} MATCHES "amd64")
  file(GLOB sources ${ArduinoMDNS_PATH}/*.cpp ${ArduinoMDNS_PATH}/utility/*.c ${CMAKE_SOURCE_DIR}/third-party/arduino-stubs/*.cpp)
  add_library(ArduinoMDNS STATIC ${sources})
  target_include_directories(ArduinoMDNS
	PUBLIC ${ArduinoMDNS_PATH}
	PRIVATE ${ArduinoMDNS_PATH}
  )
  target_include_directories(ArduinoMDNS
    PUBLIC ${CMAKE_SOURCE_DIR}/third-party/arduino-stubs
    PRIVATE ${CMAKE_SOURCE_DIR}/third-party/arduino-stubs
  )
else()
  file(GLOB sources ${ArduinoMDNS_PATH}/*.cpp ${ArduinoMDNS_PATH}/utility/*.c)
  add_arduino_library(ArduinoMDNS "${sources}")
  target_include_directories(ArduinoMDNS
	PUBLIC ${ArduinoMDNS_PATH}
	PRIVATE ${ArduinoMDNS_PATH}
  )
endif()
