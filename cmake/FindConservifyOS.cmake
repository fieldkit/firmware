#
#
#
set(ConservifyOS_PATH ${CMAKE_SOURCE_DIR}/libraries/arduino-osh/src)

file(GLOB sources ${ConservifyOS_PATH}/*.c ${ConservifyOS_PATH}/*.s ${ConservifyOS_PATH}/*.cpp)

add_arduino_library(ConservifyOS "${sources}")

find_package(SeggerRTT)
target_link_libraries(ConservifyOS SeggerRTT)

target_include_directories(ConservifyOS
  PUBLIC ${ConservifyOS_PATH}
  PRIVATE ${ConservifyOS_PATH}
)
