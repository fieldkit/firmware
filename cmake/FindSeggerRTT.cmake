#
#
#
if(TARGET SeggerRTT)
  return()
endif()

set(SeggerRTT_PATH ${CMAKE_SOURCE_DIR}/libraries/conservify/segger)

file(GLOB sources ${SeggerRTT_PATH}/*.c)
if(${TARGET_ARCH} MATCHES "amd64")
  add_library(SeggerRTT STATIC "${sources}")
else()
  add_arduino_library(SeggerRTT "${sources}")
endif()

target_include_directories(SeggerRTT
  PUBLIC ${SeggerRTT_PATH}
  PRIVATE ${SeggerRTT_PATH}
)
