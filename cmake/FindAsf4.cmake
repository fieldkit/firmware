#
#
#
if(TARGET Asf4)
  return()
endif()

set(asf4_PATH ${CMAKE_SOURCE_DIR}/third-party/asf4)

file(GLOB sources
  ${asf4_PATH}/samd51/hal/src/*.c
  ${asf4_PATH}/samd51/hpl/*/*.c
  ${asf4_PATH}/samd51/driver*.c
)

add_arduino_library(Asf4 "${sources}")

target_include_directories(Asf4
  PUBLIC
  ${asf4_PATH}/samd51/config
  ${asf4_PATH}/samd51/include ${asf4_PATH}/samd51/hri
  ${asf4_PATH}/samd51/hal/include
  ${asf4_PATH}/samd51/hal/utils/include
  ${asf4_PATH}/samd51/hpl/core
  ${asf4_PATH}/samd51/hpl/gclk
  ${asf4_PATH}/samd51/hpl/wdt
  ${asf4_PATH}/samd51/hpl/port
  ${asf4_PATH}/samd51/hpl/tc
  ${asf4_PATH}/samd51
)
