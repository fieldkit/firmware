if(TARGET lwstreams)
  return()
endif()

if(${TARGET_ARCH} MATCHES "amd64")
  file(GLOB sources ${lwstreams_PATH}/src/lwstreams/*.cpp)

  add_library(lwstreams STATIC ${sources})

  target_include_directories(lwstreams PUBLIC ${lwstreams_PATH}/src)
else()
  set(lwstreams_RECURSE True)

  add_external_arduino_library(lwstreams)
endif()
