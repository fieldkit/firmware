#
#
#
if(${TARGET_ARCH} MATCHES "amd64")
  add_subdirectory(fake)
  add_subdirectory(hosted)
  add_subdirectory(fstool)
endif()

if(${TARGET_ARCH} MATCHES "samd51")
  add_subdirectory(metal)
endif()
