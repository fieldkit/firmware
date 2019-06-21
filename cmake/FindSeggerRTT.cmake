#
#
#
if(TARGET SeggerRTT)
  return()
endif()

set(SeggerRTT_PATH ${CMAKE_SOURCE_DIR}/libraries/SeggerRTT)

file(GLOB sources ${SeggerRTT_PATH}/*.c)

add_arduino_library(SeggerRTT "${sources}")

target_include_directories(SeggerRTT
  PUBLIC ${SeggerRTT_PATH}
  PRIVATE ${SeggerRTT_PATH}
)
