#
#
#
if(TARGET Lfs)
  return()
endif()

set(lfs_PATH ${CMAKE_SOURCE_DIR}/third-party/littlefs)

if(${TARGET_ARCH} MATCHES "amd64")
  file(GLOB sources ${lfs_PATH}/*.c ${lfs_PATH}/*.h ${lfs_PATH}/bd/*.c ${lfs_PATH}/bd/*.h)
  add_library(Lfs STATIC ${sources})
else()
  file(GLOB sources ${lfs_PATH}/*.c ${lfs_PATH}/*.h)
  add_arduino_library(Lfs "${sources}")
endif()

target_include_directories(Lfs PUBLIC ${lfs_PATH})
