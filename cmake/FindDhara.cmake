#
#
#
if(TARGET Dhara)
  return()
endif()

set(dhara_PATH ${CMAKE_SOURCE_DIR}/third-party/dhara)

if(${TARGET_ARCH} MATCHES "amd64")
  file(GLOB sources ${dhara_PATH}/dhara/*.c ${dhara_PATH}/dhara/*.h)
  add_library(Dhara STATIC ${sources})
else()
  file(GLOB sources ${dhara_PATH}/dhara/*.c ${dhara_PATH}/dhara/*.h)
  add_arduino_library(Dhara "${sources}")
endif()

target_include_directories(Dhara PUBLIC ${dhara_PATH})
