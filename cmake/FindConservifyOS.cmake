#
#
#
if(TARGET ConservifyOS)
  return()
endif()

set(ConservifyOS_PATH ${arduino-osh_PATH}/src)

file(GLOB sources ${ConservifyOS_PATH}/*.c ${ConservifyOS_PATH}/*.s ${ConservifyOS_PATH}/*.cpp)

if(${TARGET_ARCH} MATCHES "amd64")
  add_library(ConservifyOS STATIC "${sources}")
else()
  add_arduino_library(ConservifyOS "${sources}")
endif()

find_package(SeggerRTT)
target_link_libraries(ConservifyOS SeggerRTT)

target_compile_options(ConservifyOS PRIVATE -Wall -Werror) # -DOS_CONFIG_DEBUG_SCHEDULE)

target_include_directories(ConservifyOS
  PUBLIC ${ConservifyOS_PATH}
  PRIVATE ${ConservifyOS_PATH}
)
