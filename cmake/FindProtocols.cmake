#
#
#
if(TARGET Protocols)
  return()
endif()

if(${TARGET_ARCH} MATCHES "amd64")
  file(GLOB sources ${app-protocol_PATH}/src/*.c)
  add_library(app-protocol STATIC ${sources})
  target_include_directories(app-protocol
    PUBLIC ${app-protocol_PATH}/src
    PRIVATE ${app-protocol_PATH}/src
  )
else()
  add_external_arduino_library(app-protocol)
endif()

find_package(nanopb)
target_link_libraries(app-protocol nanopb)
