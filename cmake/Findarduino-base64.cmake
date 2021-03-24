if (TARGET arduino-base64)
  return()
endif()

if(${TARGET_ARCH} MATCHES "amd64")
  file(GLOB sources ${arduino-base64_PATH}/*.cpp)

  add_library(arduino-base64 STATIC ${sources})

  target_include_directories(arduino-base64
    PUBLIC ${arduino-base64_PATH}
    PRIVATE ${arduino-base64_PATH}
  )
else()
  add_external_arduino_library(arduino-base64)
endif()
