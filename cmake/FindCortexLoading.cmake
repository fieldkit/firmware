#
#
#
if(TARGET CortexLoading)
  return()
endif()

set(CortexLoading_PATH ${loading_PATH})

if(${TARGET_ARCH} MATCHES "amd64")
  file(GLOB sources ${CortexLoading_PATH}/src/external.c)
  add_library(CortexLoading STATIC ${sources})
else()
  file(GLOB sources ${CortexLoading_PATH}/src/*.c ${CortexLoading_PATH}/src/*.s ${CortexLoading_PATH}/src/*.cpp)
  add_arduino_library(CortexLoading "${sources}")
endif()

find_package(SeggerRTT)
target_link_libraries(CortexLoading SeggerRTT)

target_include_directories(CortexLoading
  PUBLIC ${CortexLoading_PATH}/include
  PRIVATE ${CortexLoading_PATH}/include
)
