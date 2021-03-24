if(TARGET simple-lora-comms)
  return()
endif()

add_external_arduino_library(simple-lora-comms)

find_package(arduino-logging)
target_link_libraries(simple-lora-comms arduino-logging)

find_package(nanopb)
target_link_libraries(simple-lora-comms nanopb)

find_package(lwstreams)
target_link_libraries(simple-lora-comms lwstreams)

find_package(RadioHead)
target_link_libraries(simple-lora-comms RadioHead)
