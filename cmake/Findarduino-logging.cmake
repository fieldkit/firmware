if (TARGET arduino-logging)
  return()
endif()

if(${TARGET_ARCH} MATCHES "amd64")
  file(GLOB sources ${arduino-logging_PATH}/src/*.cpp)

  add_library(arduino-logging STATIC ${sources})

  target_include_directories(arduino-logging PUBLIC ${arduino-logging_PATH}/src)
else()
  add_external_arduino_library(arduino-logging)
endif()
