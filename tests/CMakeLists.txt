#
#
#
if(${TARGET_ARCH} MATCHES "amd64")
  add_subdirectory(hosted)
endif()

if(${TARGET_ARCH} MATCHES "samd51")
  add_subdirectory(metal)
endif()
