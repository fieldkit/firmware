#
#
#
if(TARGET TheThingsNetwork)
  return()
endif()

file(GLOB sources ${TheThingsNetwork_PATH}/src/*.cpp)

add_arduino_library(TheThingsNetwork "${sources}")

target_include_directories(TheThingsNetwork
  PUBLIC ${TheThingsNetwork_PATH}/src
  PRIVATE ${TheThingsNetwork_PATH}/src
)
