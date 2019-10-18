#
#
#
if(TARGET Samd51Common)
  return()
endif()

set(Samd51Common_PATH ${CMAKE_SOURCE_DIR}/third-party/samd51-common)

file(GLOB sources ${Samd51Common_PATH}/*.c)

if(${TARGET_ARCH} MATCHES "amd64")
  add_library(Samd51Common STATIC "${sources}")
else()
  add_arduino_library(Samd51Common "${sources}")

  find_package(Asf4)
  target_link_libraries(Samd51Common Asf4)
endif()

target_include_directories(Samd51Common PUBLIC ${Samd51Common_PATH})
