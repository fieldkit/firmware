if(${TARGET_ARCH} MATCHES "samd51")
  add_subdirectory(main)
endif()

if(${TARGET_ARCH} MATCHES "samd09")
  add_subdirectory(sidecar)
endif()

