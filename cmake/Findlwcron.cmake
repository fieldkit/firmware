if(TARGET lwcron)
  return()
endif()

if(${TARGET_ARCH} MATCHES "amd64")
  file(GLOB sources ${lwcron_PATH}/src/lwcron/*.cpp)

  add_library(lwcron STATIC ${sources})

  target_include_directories(lwcron
    PUBLIC ${lwcron_PATH}/src
    PRIVATE ${lwcron_PATH}/src
  )
else()
  set(lwcron_RECURSE True)
  add_external_arduino_library(lwcron)
endif()
