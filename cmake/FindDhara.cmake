if (TARGET Dhara)
  return()
endif()

if(${TARGET_ARCH} MATCHES "samd51")
  add_external_arduino_library(Dhara)
else()
  file(GLOB sources ${CMAKE_CURRENT_LIST_DIR}/../third-party/dhara/dhara/*.c ${CMAKE_CURRENT_LIST_DIR}/third-party/dhara/dhara/*.h)

  add_library(Dhara STATIC ${sources})

  message( ${CMAKE_CURRENT_LIST_DIR}/../third-party/dhara)
  target_include_directories(Dhara PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../third-party/dhara)
endif()
