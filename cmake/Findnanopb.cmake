if (TARGET nanopb)
  return()
endif()

if(${TARGET_ARCH} MATCHES "amd64")
  file(GLOB sources ${nanopb_PATH}/*.c)

  add_library(nanopb STATIC ${sources})

  target_include_directories(nanopb
    PUBLIC ${nanopb_PATH}
    PRIVATE ${nanopb_PATH}
  )
else()
  add_external_arduino_library(nanopb)
endif()
