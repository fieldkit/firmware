#
#
#
if(TARGET SleepyDog)
  return()
endif()

set(SleepyDog_PATH ${Adafruit_SleepyDog_PATH})

if(${TARGET_ARCH} MATCHES "amd64")
else()
  file(GLOB sources ${SleepyDog_PATH}/*.h ${SleepyDog_PATH}/*.cpp ${SleepyDog_PATH}/utility/*.h ${SleepyDog_PATH}/utility/*.cpp)
  add_arduino_library(SleepyDog "${sources}")
endif()

target_include_directories(SleepyDog
  PUBLIC ${SleepyDog_PATH}/
  PRIVATE ${SleepyDog_PATH}/
)
