if(TARGET RTClib)
  return()
endif()

add_external_arduino_library(RTClib)

find_package(Wire)

target_link_libraries(RTClib Wire)
