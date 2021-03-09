#
#
#
if(TARGET Lfs)
  return()
endif()

set(lfs_PATH ${CMAKE_SOURCE_DIR}/third-party/littlefs)

file(GLOB sources ${lfs_PATH}/*.c ${lfs_PATH}/*.h)

if(${TARGET_ARCH} MATCHES "amd64")
  add_library(Lfs STATIC ${sources})
else()
  add_arduino_library(Lfs "${sources}")
endif()

target_include_directories(Lfs PUBLIC ${lfs_PATH})
