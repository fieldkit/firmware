#
#
#
set(CortexLoading_PATH ${CMAKE_SOURCE_DIR}/libraries/jlewallen/loading)

if(TARGET CortexLoading)
  return()
endif()

file(GLOB sources ${CortexLoading_PATH}/src/*.c ${CortexLoading_PATH}/src/*.s ${CortexLoading_PATH}/src/*.cpp)

add_arduino_library(CortexLoading "${sources}")

find_package(SeggerRTT)
target_link_libraries(CortexLoading SeggerRTT)

target_compile_options(CortexLoading PRIVATE -DFKB_EXTERNAL_NEED_PRINTF)

target_include_directories(CortexLoading
  PUBLIC ${CortexLoading_PATH}/include
  PRIVATE ${CortexLoading_PATH}/include
)
